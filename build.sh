#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")"

# this is opinionated; sorry
BUILD_TYPE=Release
PROJECTS=(
	-DBuildJK2SPGame=ON
	-DBuildSPGame=ON
)
EXTRA_OPTS=(
	-DCMAKE_EXPORT_COMPILE_COMMANDS=ON
	-DCMAKE_INSTALL_PREFIX="${CMAKE_INSTALL_PREFIX:-~/games/openjk}"
)

ARGS=("$@")
for ((i = 0; i < ${#ARGS[@]}; i++)); do
	case ${ARGS[$i],,} in
	"debug")
		BUILD_TYPE=Debug
		;;
	"fastdebug" | "relwithdebinfo")
		BUILD_TYPE=RelWithDebInfo
		;;
	"mingw")
		EXTRA_OPTS+=("-DCMAKE_TOOLCHAIN_FILE=CMakeModules/Toolchains/i686-w64-mingw32.cmake")
		;;
	*) ;;
	esac
done

mkdir -p "build-$BUILD_TYPE"
pushd "build-$BUILD_TYPE" >/dev/null && {
	cmake -G "Unix Makefiles" "-DCMAKE_BUILD_TYPE=$BUILD_TYPE" "${PROJECTS[@]}" "${EXTRA_OPTS[@]}" ../
	make "-j$(($(nproc || true) * 2))"
	make install
	popd >/dev/null
}
