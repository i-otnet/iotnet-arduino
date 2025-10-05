#!/usr/bin/env python3
import json
import subprocess
import re
import sys
from pathlib import Path


# ======================
# Utility: Parse version string safely
# ======================
def version_to_tuple(version):
    """
    Convert version string to numeric tuple safely.
    Examples:
      '1.0.0-dev'  -> (1, 0, 0)
      '2.3.1-beta' -> (2, 3, 1)
      '3.0'        -> (3, 0)
    """
    clean_parts = []
    for part in version.split('.'):
        # Buang suffix seperti -dev, -beta, -rc1, dsb
        clean = re.split(r'[-_+]', part)[0]
        try:
            clean_parts.append(int(clean))
        except ValueError:
            clean_parts.append(0)  # fallback
    return tuple(clean_parts)


# ======================
# Get latest git tag (or default)
# ======================
def get_last_git_tag():
    try:
        tag = subprocess.check_output(
            ["git", "describe", "--tags", "--abbrev=0"], stderr=subprocess.DEVNULL
        ).decode().strip()
        return tag.lstrip("v")  # e.g. "v1.2.3" → "1.2.3"
    except subprocess.CalledProcessError:
        return "0.0.0"


# ======================
# Main
# ======================
def main():
    root = Path(__file__).resolve().parent.parent

    # File paths
    json_file = root / "library.json"
    props_file = root / "library.properties"

    # Load library.json
    v_json = "0.0.0"
    if json_file.exists():
        try:
            with open(json_file) as f:
                data = json.load(f)
                v_json = data.get("version", "0.0.0")
        except Exception as e:
            print(f"⚠️  Error reading library.json: {e}")

    # Load library.properties
    v_props = "0.0.0"
    if props_file.exists():
        try:
            with open(props_file) as f:
                for line in f:
                    if line.startswith("version="):
                        v_props = line.split("=")[1].strip()
                        break
        except Exception as e:
            print(f"⚠️  Error reading library.properties: {e}")

    # Get latest git tag
    v_git = get_last_git_tag()

    print(f"🧩 library.json: {v_json}")
    print(f"🧩 library.properties: {v_props}")
    print(f"🏷️ last git tag: v{v_git}")

    # Compare versions
    try:
        t_json = version_to_tuple(v_json)
        t_props = version_to_tuple(v_props)
        t_git = version_to_tuple(v_git)

        if not (t_json == t_props and t_json > t_git):
            print("❌ Version inconsistency detected!")
            print(f"   library.json:       {v_json}")
            print(f"   library.properties: {v_props}")
            print(f"   last git tag:       v{v_git}")
            sys.exit(1)

        print("✅ Version check passed.")
        sys.exit(0)
    except Exception as e:
        print(f"⚠️  Version comparison failed: {e}")
        sys.exit(1)


if __name__ == "__main__":
    main()
