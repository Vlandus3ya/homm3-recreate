import os
import tempfile
import unittest
import zipfile

from tools.recreate_fim import build_report, map_state


class FimTests(unittest.TestCase):
    def test_map_curation(self):
        self.assertEqual(map_state("Maps/(CAMPAIGN) Moldovanka.h3m"), "excluded")
        self.assertEqual(map_state("Maps/World adventures.h3m"), "in_development")
        self.assertIsNone(map_state("Maps/readme.txt"))

    def test_zip_audit_and_duplicate_detection(self):
        with tempfile.TemporaryDirectory() as temp:
            archive_path = os.path.join(temp, "data.zip")
            with zipfile.ZipFile(archive_path, "w") as archive:
                archive.writestr("Maps/World adventures.h3m", b"mapdata")
                archive.writestr("copy/map.bin", b"mapdata")
            report = build_report(archive_path)
            self.assertEqual(report["archive_test"], "passed")
            self.assertEqual(report["files_scanned"], 2)
            self.assertEqual(len(report["duplicate_content"]), 1)
            self.assertEqual(report["maps"][0]["curation"], "in_development")
            self.assertEqual(report["status"], "clean")

    def test_case_insensitive_collision(self):
        with tempfile.TemporaryDirectory() as temp:
            archive_path = os.path.join(temp, "collision.zip")
            with zipfile.ZipFile(archive_path, "w") as archive:
                archive.writestr("Data/File.bin", b"a")
                archive.writestr("data/file.bin", b"b")
            report = build_report(archive_path)
            self.assertEqual(len(report["path_collisions"]), 1)
            self.assertEqual(report["status"], "issues_found")


if __name__ == "__main__":
    unittest.main()
