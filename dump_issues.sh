#!/usr/bin/env bash

set -eu -o pipefail

OUTDIR=issues
LIMIT=999
ISSUE_FIELDS="body,closed,comments,labels,milestone,projectCards,projectItems,state,title"

mkdir -p "$OUTDIR"

gh issue list --json number --limit "$LIMIT" --jq '.[].number' | while read number; do
    echo "fetching issue $number..."
    gh issue view "$number" --json "$ISSUE_FIELDS" > "$OUTDIR/$number.json"
done
