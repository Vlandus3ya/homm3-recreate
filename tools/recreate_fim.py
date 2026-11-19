#!/usr/bin/env python3
"""Read-only integrity audit for Heroes III: Recreate game-data sources.

The tool never extracts, edits, deletes, or overwrites source files. It accepts
ZIP archives and directories and can emit a JSON report for the Recreate UI.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
import sys
import zipfile
from collections import defaultdict
from pathlib import PurePosixPath
from typing import Iterable

EXCLUDED_MAPS = {"(campaign) moldovanka.h3m"}
DEVELOPMENT_MAPS = {"world adventures.h3m"}
CHUNK_SIZE = 1024 * 1024


def sha256_stream(stream) -> str:
    digest = hashlib.sha256()
    while True:
        chunk = stream.read(CHUNK_SIZE)
        if not chunk:
            break
        digest.update(chunk)
    return digest.hexdigest()


def safe_archive_name(name: str) -> bool:
    normalized = name.replace("\\", "/")
    path = PurePosixPath(normalized)
    return not path.is_absolute() and ".." not in path.parts and not (len(normalized) > 1 and normalized[1] == ":")


def map_state(name: str) -> str | None:
    base = PurePosixPath(name.replace("\\", "/")).name.casefold()
    if not base.endswith(".h3m"):
        return None
    if base in EXCLUDED_MAPS:
        return "excluded"
    if base in DEVELOPMENT_MAPS:
        return "in_development"
    return "available_for_review"


def build_report(source: str) -> dict:
    report = {
        "schema": "recreate-fim-report/v1",
        "source": os.path.abspath(source),
        "read_only": True,
        "archive_test": "not_applicable",
        "files_scanned": 0,
        "total_bytes": 0,
        "errors": [],
        "unsafe_paths": [],
        "path_collisions": [],
        "duplicate_content": [],
        "maps": [],
    }
    records: list[dict] = []

    if zipfile.is_zipfile(source):
        report["source_type"] = "zip"
        with zipfile.ZipFile(source, "r") as archive:
            bad_member = archive.testzip()
            report["archive_test"] = "passed" if bad_member is None else "failed"
            if bad_member is not None:
                report["errors"].append(f"CRC check failed: {bad_member}")
            seen: dict[str, str] = {}
            for info in archive.infolist():
                if info.is_dir():
                    continue
                name = info.filename
                if not safe_archive_name(name):
                    report["unsafe_paths"].append(name)
                    continue
                key = name.replace("\\", "/").casefold()
                if key in seen:
                    report["path_collisions"].append({"first": seen[key], "second": name})
                else:
                    seen[key] = name
                with archive.open(info, "r") as stream:
                    digest = sha256_stream(stream)
                records.append({"path": name, "size": info.file_size, "sha256": digest})
    elif os.path.isdir(source):
        report["source_type"] = "directory"
        root = os.path.abspath(source)
        for current, dirs, files in os.walk(root, followlinks=False):
            dirs[:] = sorted(d for d in dirs if not os.path.islink(os.path.join(current, d)))
            for filename in sorted(files):
                path = os.path.join(current, filename)
                if os.path.islink(path):
                    report["errors"].append(f"Symbolic link skipped: {os.path.relpath(path, root)}")
                    continue
                relative = os.path.relpath(path, root).replace(os.sep, "/")
                if not safe_archive_name(relative):
                    report["unsafe_paths"].append(relative)
                    continue
                with open(path, "rb") as stream:
                    digest = sha256_stream(stream)
                records.append({"path": relative, "size": os.path.getsize(path), "sha256": digest})
    else:
        raise ValueError("Source must be an existing ZIP archive or directory.")

    report["files_scanned"] = len(records)
    report["total_bytes"] = sum(item["size"] for item in records)

    by_hash: dict[str, list[str]] = defaultdict(list)
    for item in records:
        by_hash[item["sha256"]].append(item["path"])
        state = map_state(item["path"])
        if state:
            report["maps"].append({
                "path": item["path"],
                "size": item["size"],
                "sha256": item["sha256"],
                "curation": state,
            })

    report["duplicate_content"] = [
        {"sha256": digest, "paths": paths}
        for digest, paths in sorted(by_hash.items()) if len(paths) > 1
    ]
    report["maps"].sort(key=lambda item: item["path"].casefold())
    report["status"] = "issues_found" if (
        report["errors"] or report["unsafe_paths"] or report["path_collisions"]
        or report["archive_test"] == "failed"
    ) else "clean"
    return report


def main(argv: Iterable[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description="Read-only Recreate File Integrity Manager audit")
    parser.add_argument("source", help="ZIP archive or directory containing game data")
    parser.add_argument("--json", dest="json_path", help="write the report to this JSON file")
    args = parser.parse_args(argv)

    try:
        report = build_report(args.source)
    except (OSError, zipfile.BadZipFile, ValueError) as exc:
        print(f"FIM error: {exc}", file=sys.stderr)
        return 2

    print(f"Recreate FIM: {report['status']}")
    print(f"Files scanned: {report['files_scanned']}")
    print(f"Total size: {report['total_bytes']:,} bytes")
    print(f"Maps found: {len(report['maps'])}")
    print(f"Duplicate-content groups: {len(report['duplicate_content'])}")
    print(f"Path collisions: {len(report['path_collisions'])}")
    print(f"Unsafe paths: {len(report['unsafe_paths'])}")
    print(f"Archive CRC: {report['archive_test']}")
    for item in report["maps"]:
        if item["curation"] in {"excluded", "in_development"}:
            print(f"Map [{item['curation']}]: {PurePosixPath(item['path']).name}")
    if args.json_path:
        with open(args.json_path, "w", encoding="utf-8", newline="\n") as output:
            json.dump(report, output, ensure_ascii=False, indent=2)
            output.write("\n")
        print(f"JSON report: {os.path.abspath(args.json_path)}")
    return 1 if report["status"] != "clean" else 0


if __name__ == "__main__":
    raise SystemExit(main())
