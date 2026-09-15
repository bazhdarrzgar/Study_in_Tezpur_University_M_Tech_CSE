#!/bin/bash

# Navigate to the repository root regardless of where the script is executed
REPO_ROOT="$(git rev-parse --show-toplevel 2>/dev/null)"

if [ -z "$REPO_ROOT" ]; then
    echo "Error: Not inside a Git repository."
    exit 1
fi

cd "$REPO_ROOT" || exit 1

# Check if there are any changes (modified, deleted, untracked)
if [ -z "$(git status --porcelain)" ]; then
    echo "No changes detected. Working tree is clean!"
    exit 0
fi

# Display current changes
echo "================ Current Changes ================"
git status -s
echo "================================================="

# Ask for the commit message
echo ""
read -r -p "Enter commit message: " commit_msg

# Ensure commit message is not empty
while [ -z "$commit_msg" ]; do
    echo "Commit message cannot be empty."
    read -r -p "Enter commit message: " commit_msg
done

# Stage all changes
echo ""
echo "Staging changes..."
git add -A

# Commit
echo "Committing..."
git commit -m "$commit_msg"

# Detect current active branch
CURRENT_BRANCH="$(git branch --show-current)"
if [ -z "$CURRENT_BRANCH" ]; then
    CURRENT_BRANCH="main"
fi

# Push to GitHub
echo "Pushing to GitHub (origin/$CURRENT_BRANCH)..."
git push origin "$CURRENT_BRANCH"

if [ $? -eq 0 ]; then
    echo ""
    echo "Successfully pushed changes to GitHub!"
else
    echo ""
    echo "Push failed. Please check any error messages above."
    exit 1
fi
