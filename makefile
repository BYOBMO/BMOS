OBJS=CFiles.o CFaceFile.o CMessageBox.o CTextReader.o CMultiLine.o CSurface.o CSurfWindow.o CVScrollBar.o CListBox.o CFolderForm.o CListBoxItem.o CDragWindow.o CLabel.o CButton.o CRectangle.o CText.o CTexture.o CWindow.o CDesktop.o CForm.o CFolder.o CIcon.o CMenuBar.o CMenuBarItem.o CPanel.o CTitleBar.o CDropDownMenu.o CSettingsForm.o
CC=g++

CFLAGS = -Wall `sdl2-config --cflags --libs`

LFLAGS = -lSDL2 -lSDL2_mixer -lSDL2_ttf -lSDL2_image 

bmos: $(OBJS) main.o
	$(CC) $(CFLAGS)  $(OBJS) main.o $(LFLAGS) -o bmos

main.o: main.cpp 
	$(CC) -c $(CFLAGS) -I ./ -I ./SDL main.cpp -o main.o


CRectangle.o: CRectangle.cpp 
	$(CC) -c $(CFLAGS) -I ./ -I ./SDL CRectangle.cpp -o CRectangle.o

CText.o: CText.cpp 
	$(CC) -c $(CFLAGS) -I ./ -I ./SDL  CText.cpp -o CText.o

CTexture.o: CTexture.cpp 
	$(CC) -c $(CFLAGS) -I ./ -I ./SDL  CTexture.cpp -o CTexture.o

CWindow.o: CWindow.cpp 
	$(CC) -c $(CFLAGS) -I ./ -I ./SDL  CWindow.cpp -o CWindow.o

CDesktop.o: CDesktop.cpp 
	$(CC) -c $(CFLAGS) -I ./ -I ./SDL  CDesktop.cpp -o CDesktop.o

CFolder.o: CFolder.cpp 
	$(CC) -c $(CFLAGS) -I ./ -I ./SDL  CFolder.cpp -o CFolder.o

CForm.o: CForm.cpp 
	$(CC) -c $(CFLAGS) -I ./ -I ./SDL  CForm.cpp -o CForm.o

CIcon.o: CIcon.cpp 
	$(CC) -c $(CFLAGS) -I ./ -I ./SDL  CIcon.cpp -o CIcon.o

CMenuBar.o: CMenuBar.cpp 
	$(CC) -c $(CFLAGS) -I ./ -I ./SDL  CMenuBar.cpp -o CMenuBar.o

CMenuBarItem.o: CMenuBarItem.cpp 
	$(CC) -c $(CFLAGS) -I ./ -I ./SDL  CMenuBarItem.cpp -o CMenuBarItem.o

CPanel.o: CPanel.cpp 
	$(CC) -c $(CFLAGS) -I ./ -I ./SDL  CPanel.cpp -o CPanel.o

CTitleBar.o: CTitleBar.cpp 
	$(CC) -c $(CFLAGS) -I ./ -I ./SDL  CTitleBar.cpp -o CTitleBar.o

CDropDownMenu.o: CDropDownMenu.cpp 
	$(CC) -c $(CFLAGS) -I ./ -I ./SDL  CDropDownMenu.cpp -o CDropDownMenu.o

CSettingsForm.o: CSettingsForm.cpp 
	$(CC) -c $(CFLAGS) -I ./ -I ./SDL  CSettingsForm.cpp -o CSettingsForm.o

CButton.o: CButton.cpp 
	$(CC) -c $(CFLAGS) -I ./ -I ./SDL  CButton.cpp -o CButton.o

CLabel.o: CLabel.cpp 
	$(CC) -c $(CFLAGS) -I ./ -I ./SDL  CLabel.cpp -o CLabel.o

CDragWindow.o: CDragWindow.cpp 
	$(CC) -c $(CFLAGS) -I ./ -I ./SDL  CDragWindow.cpp -o CDragWindow.o

CListBoxItem.o: CListBoxItem.cpp 
	$(CC) -c $(CFLAGS) -I ./ -I ./SDL  CListBoxItem.cpp -o CListBoxItem.o

CListBox.o: CListBox.cpp 
	$(CC) -c $(CFLAGS) -I ./ -I ./SDL  CListBox.cpp -o CListBox.o

CFolderForm.o: CFolderForm.cpp 
	$(CC) -c $(CFLAGS) -I ./ -I ./SDL  CFolderForm.cpp -o CFolderForm.o

CVScrollBar.o: CVScrollBar.cpp 
	$(CC) -c $(CFLAGS) -I ./ -I ./SDL  CVScrollBar.cpp -o CVScrollBar.o

CSurface.o: CSurface.cpp 
	$(CC) -c $(CFLAGS) -I ./ -I ./SDL  CSurface.cpp -o CSurface.o

CSurfWindow.o: CSurfWindow.cpp 
	$(CC) -c $(CFLAGS) -I ./ -I ./SDL  CSurfWindow.cpp -o CSurfWindow.o

CMultiLine.o: CMultiLine.cpp 
	$(CC) -c $(CFLAGS) -I ./ -I ./SDL  CMultiLine.cpp -o CMultiLine.o

CMessageBox.o: CMessageBox.cpp 
	$(CC) -c $(CFLAGS) -I ./ -I ./SDL  CMessageBox.cpp -o CMessageBox.o

CTextReader.o: CTextReader.cpp 
	$(CC) -c $(CFLAGS) -I ./ -I ./SDL  CTextReader.cpp -o CTextReader.o

CFaceFile.o: CFaceFile.cpp 
	$(CC) -c $(CFLAGS) -I ./ -I ./SDL  CFaceFile.cpp -o CFaceFile.o

CFiles.o: CFiles.cpp 
	$(CC) -c $(CFLAGS) -I ./ -I ./SDL  CFiles.cpp -o CFiles.o

clean:
	rm $(OBJS)
	rm main.o
