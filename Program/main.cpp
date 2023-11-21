/*
osu! hitsound updater (o!hsu) - by Anthony Garcia Roman
Last edited 21/11/2023.
*/

#include <sys\stat.h>
#include <iostream>
#include <fstream>
#include "Program/DirLookup.h"

using namespace std;

int main() {
    //Skin to be edited
    string clientSkin = "";
    //Skin to take hitsounds from
    string hostSkin = "";

    DrLookup();


    cout
    <<
    "----------------\n"
    <<
    "From this list, which skin do you want to change the hitsounds of? (case & white space sensitive)\n";
    cin >> clientSkin;
    cout << "Which skin do you want to take the hitsounds from? (case & white space sensitive)\n";
    cin >> hostSkin;

    //turn into pointers so can be addressed in SkinLookup.h to search
    //inside the skin files for the hitsounds.
    &clientSkin = *clientSkinPtr;
    string &&clientSkinPtr = osuSkinFolder + "\\" + clientSkin;
    &hostSkin = *hostSkinPtr;
    string &&hostSkinPtr = osuSkinFolder + "\\" + hostSkin;


    return 0;

}