#============================================================================
# Copyright (C) 2015, OpenJK contributors
#
# This file is part of the OpenJK source code.
#
# OpenJK is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, see <http://www.gnu.org/licenses/>.
#============================================================================

# Subdirectories to package JK2 and JKA into
set(JKAInstallDir "JediAcademy")
set(JK2InstallDir "JediOutcast")

# Install components
set(JKASPClientComponent "JKASPClient")
set(JKASPAssetsComponent "JKASPAssets")
set(JK2SPClientComponent "JK2SPClient")

# Component display names
include(CPackComponent)

set(CPACK_COMPONENT_JKASPCLIENT_DISPLAY_NAME "Core")
set(CPACK_COMPONENT_JKASPCLIENT_DESCRIPTION "Binaries required to play WtfJK on Jedi Academy single player.")
set(CPACK_COMPONENT_JKASPASSETS_DISPLAY_NAME "Core")
set(CPACK_COMPONENT_JKASPASSETS_DESCRIPTION "Assets required to play WtfJK on Jedi Academy single player.")
set(CPACK_COMPONENT_JK2SPCLIENT_DISPLAY_NAME "Core")
set(CPACK_COMPONENT_JK2SPCLIENT_DESCRIPTION "Binaries required to play WtfJK on Jedi Outcast single player.")
set(CPACK_COMPONENTS_ALL
	${JKASPAssetsComponent}
	${JKASPAssetsComponent}
	${JK2SPClientComponent})

set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)

# Component groups
set(CPACK_COMPONENT_JKASPCLIENT_GROUP "JKASP")
set(CPACK_COMPONENT_JKASPASSETS_GROUP "JKASP")
set(CPACK_COMPONENT_JK2SPCLIENT_GROUP "JK2SP")

cpack_add_component_group(JKASP
	DISPLAY_NAME "Jedi Academy Single Player"
	DESCRIPTION "Jedi Academy single player game")
cpack_add_component_group(JK2SP
	DISPLAY_NAME "Jedi Outcast Single Player"
	DESCRIPTION "Jedi Outcast single player game")

if(WIN32)
	set(CPACK_NSIS_DISPLAY_NAME "WtfJK")
	set(CPACK_NSIS_PACKAGE_NAME "WtfJK")
	set(CPACK_NSIS_MUI_ICON "${SharedDir}/icons/icon.ico")
	set(CPACK_NSIS_MUI_UNIICON "${SharedDir}/icons/icon.ico")
	set(CPACK_NSIS_URL_INFO_ABOUT "https://github.com/Razish/wtfjk")

	set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP TRUE)
	include(InstallRequiredSystemLibraries)
endif()

# CPack for installer creation
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_FILE_NAME "WtfJK-${CMAKE_SYSTEM_NAME}-${Architecture}")

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A chaotic Jedi Academy")
set(CPACK_PACKAGE_VENDOR "razor")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "OpenJK")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.txt")
set(CPACK_PACKAGE_DIRECTORY ${PACKAGE_DIR})
set(CPACK_BINARY_ZIP ON) # always create at least a zip file
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY 0) # prevent additional directory in zip

include(CPack)
