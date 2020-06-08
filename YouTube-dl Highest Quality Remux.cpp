#include <iostream>
#include <string>
#include <fstream>
#include <filesystem> // C++17 standard header file name

/* Demux Audio YouTube
    The following program will use FFMpeg to extract audio from downloaded .mp4 streams from YouTube
        1. Search within a Windows directory for .mp4 container files
        2. Get the filename for the file
        3. Get the YouTube video id from the filename
        4. Demux the 720p container
        5. Use YouTube DL to download the 1080p mp4 stream for the current video
        6. Mux the audio and video into one .mp4 container
        7. Delete the demuxed video and audio stream
*/

/* Todo:
    1. Add main menu
    2. Add settings menu
        a. Set the locaiton for the watch folder
        b. Set location for ffmpeg location
        c. Set locaiton for YouTube-dl location
        d. Change ffmpeg location so it fits with customizable options
        e. Change watch folder location so it fits with customizable options
    3. Use a text file to store settings
*/

using namespace std;
using namespace filesystem;

void Run_Program();
string ConvertPath(string path);
string AddQuotes(string path);
string UnderScoreRename(string path);
string AddDoubleQuotes(string path);
string RemoveDoubleQuotes(string path);
string RemoveUnderScores(string path);
bool CheckDirectoryExists(file_status s);
bool CheckFileExists(file_status s);
void loadSettings();
void Settings();
void CheckDelete(string item, int test);

string youtube_dl = "C:\\Users\\Tyler\\Desktop\\YouTube_Downloads\\youtube-dl.exe";
string ffmpeg_location = "C:\\Users\\Tyler\\Desktop\\YouTube_Downloads\\ffmpeg.exe";
string watchFolder = "C:\\Users\\Tyler\\Desktop\\YouTube_Downloads\\Watch_Folder\\";
string settings_file = "C:\\Users\\Tyler\\Desktop\\YouTube_Downloads\\Settings.txt";
string output_dir = "C:\\Users\\Tyler\\Desktop\\YouTube_Downloads\\_Done\\";

int main()
{
    bool boolean = false;
    string input;

    int i = 0;
    while (true)
    {
        loadSettings();

        while (!boolean)
        {
            system("cls");

            cout << "======== Main Menu ========" << endl;
            cout << "| 1. Run Progrm           |" << endl;
            cout << "===========================" << endl;
            cout << "| 0. Settings             |" << endl;
            cout << "===========================" << endl;
            cout << endl;
            cout << "Your Selection: ";
            getline(cin, input);

            if (input == "1")
            {
                boolean = true;
                Run_Program();
            }
            else if (input == "2")
            {
                boolean = true;
                Settings();
            }
        }
    }
}

void Run_Program()
{
    string input;
    string input2;
    string filename;
    string emptyVideo;
    string files[100];
    string id;
    string audioFile;
    string output;
    string output2;
    string command;
    string command2;
    string command3;
    string command4;
    int test;
    int integer;

    while (true)
    {
        bool boolean = false;
        bool boolean2 = false;
        id = "";

        while (!boolean)
        {
            system("cls");

            int i = 0;

            for (const auto entry : directory_iterator(watchFolder))
            {
                filename = ConvertPath(entry.path().string());
                cout << i << ". ";
                cout << filename << endl;
                files[i] = entry.path().string();
                i++;
            }

            cout << "\nYour Selection: ";
            getline(cin, input);
            integer = stoi(input);

            // Check the input
            for (int y = 0; y < i; y++)
            {
                if (integer == y)
                {
                    boolean = true;
                    string YouTube_Link = "https://www.youtube.com/watch?v=";
                    // Add quotes to the file location
                    //filename = AddQuotes(filename);
                    //files[integer] = AddDoubleQuotes(files[integer]);
                    //files[integer] = UnderScoreRename(files[integer]);
                    //files[integer] = RemoveDoubleQuotes(files[integer]); TEST!!
                    filename = ConvertPath(files[integer]);
                    audioFile = "C:\\Users\\Tyler\\Desktop\\YouTube_Downloads\\_Done\\output.wav";

                    system("cls");

                    // Execute FFMpeg to extract the audio
                    command = ffmpeg_location;
                    command += " -i ";
                    command += AddDoubleQuotes(files[integer]);
                    command += " ";
                    command += AddDoubleQuotes(audioFile);
                    command = AddDoubleQuotes(command);
                    //cout << "command: " << command << endl;
                    cout << "Demuxing Audio from " << files[integer] << "..." << endl;
                    cout << command << endl;
                    system(command.c_str());

                    // Parse the file name of the selected video extracting the video id
                    for (int z = filename.length() - 15; z < filename.length() - 4; z++)
                    {
                        id += filename[z];
                    }

                    system("cls");

                    // Create the YouTube_Link string
                    YouTube_Link += id;

                    // Use YouTube-dl to check for video and audio formats
                    //  Create command for youtube-dl
                    command2 = "C:\\Users\\Tyler\\Desktop\\YouTube_Downloads\\youtube-dl.exe -F ";
                    command2 += YouTube_Link;
                    command2 = AddDoubleQuotes(command2);
                    //cout << "command2: " << command2 << endl;
                    cout << "Searching for stream formats..." << endl;
                    system(command2.c_str());

                    while (!boolean2)
                    {
                        // Prompt the user to select the high definition video only stream from the list
                        cout << "Enter the stream number for the high definition video only stream" << endl;
                        cout << "Stream ID: ";
                        getline(cin, input);

                        if (!input.empty())
                        {
                            boolean2 = true;
                        }
                    }

                    system("cls");

                    emptyVideo = "C:\\Users\\Tyler\\Desktop\\YouTube_Downloads\\";
                    emptyVideo += filename;
                    emptyVideo = AddDoubleQuotes(emptyVideo);

                    // Download the video only stream
                    command3 = "C:\\Users\\Tyler\\Desktop\\YouTube_Downloads\\youtube-dl.exe -f ";
                    command3 += input;
                    command3 += " -o ";
                    command3 += emptyVideo;
                    command3 += " ";
                    command3 += YouTube_Link;
                    command3 = AddDoubleQuotes(command3);
                    //cout << "command3: " << command3 << endl;
                    cout << "Downloading high definition video only stream..." << endl;
                    system(command3.c_str());

                    string output = output_dir;
                    output += filename;
                    output = AddDoubleQuotes(output);

                    system("cls");

                    // Mux the video and audio streams
                    command4 = ffmpeg_location;
                    command4 += " -i ";
                    command4 += emptyVideo;
                    command4 += " -i ";
                    command4 += audioFile;
                    command4 += " -shortest -c:v copy -c:a aac ";
                    command4 += output;
                    command4 = AddDoubleQuotes(command4);
                    //cout << command4 << endl;
                    cout << "Mux the video and audio streams..." << endl;
                    system(command4.c_str());

                    output = "C:\\Users\\Tyler\\Desktop\\YouTube_Downloads\\_Done\\";
                    output += filename;
                    output2 = "C:\\Users\Tyler\\Desktop\\YouTube_Downloads\\_Done\\";
                    output2 += RemoveUnderScores(filename);
                    output2 = AddQuotes(output2);

                    // Rename the output file
                    rename(output.c_str(), output2.c_str());

                    // Delete the extracted audio file
                    test = remove(audioFile.c_str());

                    // Check if the audio file has been deleted
                    CheckDelete(audioFile, test);

                    // Delete the video only file
                    test = remove(emptyVideo.c_str());

                    // Check if the video only file has been deleted
                    CheckDelete(emptyVideo, test);

                    // Remove the original video
                    test = remove(files[integer].c_str());

                    // Check if the original video file has been deleted
                    CheckDelete(files[integer], test);

                    cout << "Press any key to continue..." << endl;
                    cin.get();
                }
            }
        }
    }
}

// Settings menu for customization
void Settings()
{
    bool boolean = false;
    string input;
    string input2;

    // Load settings from config file
    fstream settings;
    settings.open(settings_file.c_str());
    loadSettings();

    while (!boolean)
    {
        bool boolean2 = false;
        bool boolean3 = false;
        bool boolean4 = false;
        bool boolean5 = false;

        system("cls");
        cout << "=============== Settings ================" << endl;
        cout << "| 1. Change YouTube-dl location         |" << endl;
        cout << "| 2. Change Watch Directory location    |" << endl;
        cout << "| 3. Change FFMpeg Directory            |" << endl;
        cout << "| 4. Change Settings file location      |" << endl;
        cout << "| 5. Set Output Directory               |" << endl;
        cout << "=========================================" << endl;
        cout << "| 0. Back to main menu                  |" << endl;
        cout << "=========================================" << endl;
        cout << endl;
        cout << "YouTube-dl: " << youtube_dl << endl;
        cout << "Watch_Folder: " << watchFolder << endl;
        cout << "FFMpeg: " << ffmpeg_location << endl;
        cout << "Settings: " << settings_file << endl;
        cout << "OutputDir: " << output_dir << endl;
        cout << endl;
        cout << "Your selection: ";
        getline(cin, input);

        if (input == "1")
        {
            while (!boolean2)
            {
                cout << "YouTube-dl: " << youtube_dl << endl;
                cout << "Location: ";
                getline(cin, input2);

                // Check if the location exists
                if (CheckFileExists(status(input2)))
                {
                    youtube_dl = input2;
                    boolean2 = true;
                }
                else
                {
                    cout << "The location is not youtube-dl.exe..." << endl;
                    cin.get();
                }
            }
        }
        else if (input == "2")
        {
            while (!boolean3)
            {
                cout << "Watch Folder: " << watchFolder << endl;
                cout << "Location: ";
                getline(cin, input2);

                if (CheckDirectoryExists(status(input2)))
                {
                    watchFolder = input2;
                    boolean3 = true;
                }
                else
                {
                    cout << "The location entered does not exist..." << endl;
                    cin.get();
                }
            }
        }
        else if (input == "3")
        {
            while (!boolean4)
            {
                cout << "FFMpeg: " << ffmpeg_location << endl;
                cout << "Location: ";
                getline(cin, input2);

                if (CheckFileExists(status(input2)))
                {
                    ffmpeg_location = input2;
                    boolean4 = true;
                }
                else
                {
                    cout << "The location entered is not ffmpeg.exe..." << endl;
                    cin.get();
                }
            }
        }
        else if (input == "4")
        {
            while (!boolean5)
            {
                cout << "Settings File: " << settings_file << endl;
                cout << "Location: ";
                getline(cin, input2);

                if (CheckFileExists(status(input2)))
                {
                    settings_file = input;
                    boolean5 = true;
                }
                else
                {
                    cout << "The location is not the settings.txt file" << endl;
                    cin.get();
                }
            }
        }
        else if (input == "0")
        {
            boolean = true;
        }
        else
        {
            cout << "The input is invalid..." << endl;
        }
    }
}

void loadSettings()
{
    int i = 0;
    string input;
    fstream settings;
    settings.open(settings_file.c_str());

    while (getline(settings, input))
    {
        if (i == 0)
        {
            if (CheckFileExists(status(input)))
            {
                // The locaiton is valid
                youtube_dl = input;
            }
            else
            {
                cout << "The youtube-dl.exe file does not exist!" << endl;
                cin.get();
            }
        }
        else if (i == 1)
        {
            if (CheckFileExists(status(input)))
            {
                ffmpeg_location = input;
            }
            else
            {
                cout << "The watch_folder does not exist!" << endl;
                cin.get();
            }
        }
        else if (i == 2)
        {
            if (CheckDirectoryExists(status(input)))
            {
                watchFolder = input;
            }
            else
            {
                cout << "The ffmpeg.exe file does not exist!" << endl;
                cin.get();
            }
        }
        else if (i == 3)
        {
            if (CheckDirectoryExists(status(input)))
            {
                output_dir = input;
            }
            else
            {
                cout << "The output directory does not exist..." << endl;
            }
        }
        i++;
    }
}

bool CheckDirectoryExists(file_status s)
{
    return exists(s);
}

bool CheckFileExists(file_status s)
{
    return is_regular_file(s);
}

// Convert the filesystem::path to filename and extension
string ConvertPath(string path)
{
    string output;

    // Iterate through the string backwards and look for the last backslash
    for (int i = path.length(); i > 0; i--)
    {
        if (path[i] == '\\')
        {
            // Iterate through the current position plus one
            for (int j = i + 1; j < path.length(); j++)
            {
                output += path[j];
            }
            return output;
        }
    }
    return output;
}

string AddQuotes(string path)
{
    path.insert(0,"\'");
    path += "\'";

    return path;
}

// This function replaces spaces with underscores and renames the file
string UnderScoreRename(string path)
{
    string output;
    string path2;
    string filename;
    string command;
    int result;

    for (int i = 0; i < path.length(); i++)
    {
        if (isspace(path[i]))
            output += '_';
        else
            output += path[i];
    }

    result = rename(path.c_str(), output.c_str());

    if (result == 0)
    {
        cout << "File Renamed" << endl;
    }
    else
        cout << "Unable to rename the file." << endl;

    return output;
}

string AddDoubleQuotes(string path)
{
    path.insert(0, "\"");
    path += "\"";

    return path;
}

string RemoveDoubleQuotes(string path)
{
    string output;

    for (int i = 0; i < path.length(); i++)
    {
        if (path[i] != '\"')
            output += path[i];
    }

    return path;
}

void CheckDelete(string item, int test)
{
    if (test == 0)
        cout << item << " has been deleted successfully!" << endl;
    else
        cout << item << " was not deleted..." << endl;
}

string RemoveUnderScores(string path)
{
    string output = "";

    for (int x = 0; x < path.length(); x++)
    {
        if (path[x] == '_')
        {
            // Unless there are two spaces repeated, add a space, else, add a hyphen
            if (path[x - 1] == ' ')
            {
                output += '-';
            }
            else
                output += ' ';
        }
        else
            output += path[x];
    }
    
    return output;
}