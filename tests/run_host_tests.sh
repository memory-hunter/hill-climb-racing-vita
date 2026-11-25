#!/bin/sh
set -eu

root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
build="$root/build/host-tests"
cc=${CC:-clang}
cxx=${CXX:-clang++}
san="-fsanitize=address,undefined -fno-omit-frame-pointer"

mkdir -p "$build"

"$cc" -std=c11 -Wall -Wextra -Werror $san -I"$root/lib" -I"$root/lib/falso_jni" -pthread \
    "$root/tests/jni_refs_test.c" -o "$build/jni_refs_test"
ASAN_OPTIONS=abort_on_error=1 UBSAN_OPTIONS=halt_on_error=1 "$build/jni_refs_test"

"$cxx" -std=c++20 -Wall -Wextra -Werror $san -I"$root/lib" -I"$root/lib/falso_jni" -pthread \
    "$root/tests/preferences_ownership_test.cpp" -o "$build/preferences_ownership_test"
ASAN_OPTIONS=abort_on_error=1 UBSAN_OPTIONS=halt_on_error=1 "$build/preferences_ownership_test"

"$cxx" -std=c++17 -Wall -Wextra -Werror -Wno-unknown-pragmas $san \
    -I"$root/tests/stubs" -I"$root/lib" -I"$root/lib/falso_jni" -pthread \
    "$root/tests/audio_lifetime_test.cpp" -o "$build/audio_lifetime_test"
ASAN_OPTIONS=abort_on_error=1 UBSAN_OPTIONS=halt_on_error=1 "$build/audio_lifetime_test"

python3 "$root/tests/test_render_lifecycle.py"

if [ "$#" -gt 0 ]; then
    python3 "$root/tests/check_package.py" "$1"
fi
