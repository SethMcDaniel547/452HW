#!/bin/bash

set -u

root=$(cd "$(dirname "$0")/.." && pwd)
prg=${1:-"$root/shell"}
case "$prg" in
    */*) ;;
    *) prg="$root/$prg" ;;
esac
tmpdir=$(mktemp -d)
trap 'rm -rf "$tmpdir"' EXIT

cd "$root" || exit 1

echo "Building shell..."
if ! make -B -f GNUmakefile shell; then
    echo "build failed" >&2
    exit 1
fi

failures=0

run_case() {
    local name=$1
    local command=$2
    local expected=$3
    local unordered=${4:-false}
    local output="$tmpdir/$name.out"
    local actual="$output"
    local expected_output="$expected"

    printf '%s\n' "$command" | "$prg" >"$output" 2>&1

    if [ "$unordered" = true ]; then
        actual="$tmpdir/$name.actual.sorted"
        expected_output="$tmpdir/$name.expected.sorted"
        sort "$output" >"$actual"
        sort "$expected" >"$expected_output"
    fi

    if diff -u "$expected_output" "$actual" >/dev/null; then
        printf 'PASS %-2s %s\n' "$name" "$command"
    else
        printf 'FAIL %-2s %s\n' "$name" "$command" >&2
        diff -u "$expected_output" "$actual" >&2 || true
        failures=$((failures + 1))
    fi
}

initial_pwd=$(pwd)

printf '/etc\n' >"$tmpdir/cd-sequence.expected"
run_case "cd-sequence" 'cd /etc ; pwd' "$tmpdir/cd-sequence.expected"

printf '%s\n' "$initial_pwd" >"$tmpdir/cd-pipeline.expected"
run_case "cd-pipeline" 'cd /etc | pwd' "$tmpdir/cd-pipeline.expected"

printf '%s\n%s\n' "$initial_pwd" "$initial_pwd" >"$tmpdir/pwd-pipeline.expected"
run_case "pwd-pipeline" 'pwd | cat ; cd /etc | pwd' "$tmpdir/pwd-pipeline.expected"

printf '%s\n' "$initial_pwd" >"$tmpdir/cd-previous.expected"
run_case "cd-previous" 'cd /tmp ; cd - ; pwd' "$tmpdir/cd-previous.expected"

printf '%s\n%s\n' "$initial_pwd" "$initial_pwd" >"$tmpdir/background.expected"
run_case "background" 'pwd & cd /etc & pwd' "$tmpdir/background.expected" true
run_case "sequence-background" 'pwd ; cd /etc & pwd' "$tmpdir/background.expected"

printf '%s\n/etc\n' "$initial_pwd" >"$tmpdir/background-sequence.expected"
run_case "background-sequence" 'pwd & cd /etc ; pwd' "$tmpdir/background-sequence.expected"

printf '1 history\n' >"$tmpdir/history.expected"
run_case "history" 'history' "$tmpdir/history.expected"

printf 'redirected\n' >"$tmpdir/input.txt"
printf 'redirected\n' >"$tmpdir/redirection.expected"
run_case "redirection" "cat < $tmpdir/input.txt > $tmpdir/output.txt ; cat < $tmpdir/output.txt" "$tmpdir/redirection.expected"

printf '%s\n' "$initial_pwd" >"$tmpdir/builtin-redirection.expected"
run_case "builtin-redirection" "pwd > $tmpdir/builtin-pwd.txt ; cat < $tmpdir/builtin-pwd.txt" "$tmpdir/builtin-redirection.expected"

if [ "$failures" -ne 0 ]; then
    printf '%d shell command test(s) failed\n' "$failures" >&2
    exit 1
fi

echo "All shell command tests passed"
