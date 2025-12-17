#!/usr/bin/env python3
import subprocess
import re


def get_git_version():
    try:
        # Get latest tag
        version = subprocess.check_output(
            ['git', 'describe', '--tags', '--abbrev=0'],
            stderr=subprocess.STDOUT
        ).decode('utf-8').strip()
        # Remove 'v' prefix if present
        version = version.lstrip('v')
        return version
    except:
        return "0.0.0"


def update_version_file(version):
    with open('Godot/version.txt', 'w') as f:
        f.write(version)
    print(f"Updated version.txt to {version}")


def update_project_godot(version):
    with open('Godot/project.godot', 'r') as f:
        content = f.read()

    # Check if version exists
    if 'config/version=' in content:
        content = re.sub(
            r'config/version=".*?"',
            f'config/version="{version}"',
            content
        )
    else:
        # Add after [application]
        content = re.sub(
            r'\[application\]',
            f'[application]\n\nconfig/version="{version}"',
            content
        )

    with open('Godot/project.godot', 'w') as f:
        f.write(content)
    print(f"Updated project.godot to {version}")


if __name__ == "__main__":
    version = get_git_version()
    update_version_file(version)
    update_project_godot(version)
