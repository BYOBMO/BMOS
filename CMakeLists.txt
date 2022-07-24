cmake_minimum_required(VERSION 3.1)
project(bmos)


# Find SDL2 
#set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")
find_package(SDL2 REQUIRED)

    find_library(_SDL2TTF_LIB SDL2_ttf)
    list(APPEND SDL2_LIBRARIES ${_SDL2TTF_LIB})
    find_library(_SDL2IMAGE_LIB SDL2_image)
    list(APPEND SDL2_LIBRARIES ${_SDL2IMAGE_LIB})
    find_library(_SDL2MIXER_LIB SDL2_mixer)
    list(APPEND SDL2_LIBRARIES ${_SDL2MIXER_LIB})

include_directories(${SDL2_INCLUDE_DIRS})
find_package(Boost REQUIRED COMPONENTS filesystem regex)


# Define executable target
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2main_INCLUDE_DIRS} ${CMAKE_BINARY_DIR})
add_executable(bmos main.cpp 
CAudioForm.cpp
CSettings.cpp
CTextScroll.cpp
CTextBox.cpp
CWiFi.cpp
CKeyCommand.cpp
CKeyMap.cpp
CVoiceCommand.cpp
CVoiceProcess.cpp
CApplication.cpp
CPhoto.cpp 
CPhotoViewer.cpp 
CFile.cpp 
Utils.cpp 
CFiles.cpp 
CMessageBox.cpp 
CTextReader.cpp 
CMultiLine.cpp 
CSurface.cpp 
CSurfWindow.cpp 
CJoystick.cpp
CVScrollBar.cpp 
CListBox.cpp 
CFolderForm.cpp 
CListBoxItem.cpp 
CDragWindow.cpp 
CLabel.cpp 
CButton.cpp 
CRectangle.cpp 
CText.cpp 
CTexture.cpp 
CWindow.cpp 
CDesktop.cpp 
CForm.cpp 
CFolder.cpp 
CIcon.cpp 
CMenuBar.cpp 
CMenuBarItem.cpp 
CPanel.cpp 
CTitleBar.cpp 
CDropDownMenu.cpp
CSettingsForm.cpp)
target_link_libraries(bmos ${SDL2_LIBRARIES} Boost::filesystem)

install(TARGETS bmos DESTINATION /home/pi/bmos)
install(DIRECTORY fonts DESTINATION /home/pi/bmos)
install(DIRECTORY faces DESTINATION /home/pi/bmos)
install(DIRECTORY images DESTINATION /home/pi/bmos)
install(DIRECTORY faq DESTINATION /home/pi/bmos)
install(DIRECTORY pictures DESTINATION /home/pi/bmos)
install(DIRECTORY scores DESTINATION /home/pi/bmos)
install(DIRECTORY scripts DESTINATION /home/pi/bmos
        PATTERN "scripts/*"
        PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                    GROUP_EXECUTE GROUP_READ)
install(DIRECTORY servos DESTINATION /home/pi/bmos
        PATTERN "servos/*"
        PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                    GROUP_EXECUTE GROUP_READ)
install(DIRECTORY videos DESTINATION /home/pi/bmos)

install(FILES bmo.txt commands.conf DESTINATION /home/pi/bmos
PERMISSIONS  WORLD_WRITE GROUP_WRITE OWNER_WRITE
	WORLD_READ GROUP_READ OWNER_READ)

