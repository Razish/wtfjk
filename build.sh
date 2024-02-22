#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")"

# this is opinionated; sorry
BUILD_TYPE=Release # this gets overwritten by args
PROJECTS=()
EXTRA_OPTS=()

# parse args
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

# build opts
PROJECTS+=(
	-DBuildJK2SPGame=ON
	-DBuildSPGame=ON
)
EXTRA_OPTS+=(
	-DCMAKE_BUILD_TYPE="$BUILD_TYPE"
	-DCMAKE_INSTALL_PREFIX="${CMAKE_INSTALL_PREFIX:-~/games/openjk}"
)
if [ "${OS-}" != "Windows_NT" ]; then
	export CC="${CC-"clang"}"
	export CXX="${CXX-"clang++"}"
	# don't use make or compilation DB on winblows
	EXTRA_OPTS+=(
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON
		-G "Unix Makefiles"
	)
fi

cmake -B "build-$BUILD_TYPE" "${PROJECTS[@]}" "${EXTRA_OPTS[@]}"
cmake --build "build-$BUILD_TYPE" --config "$BUILD_TYPE" -j "$(nproc || true)"
cmake --install "build-$BUILD_TYPE" --config "$BUILD_TYPE"
