#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
using namespace std;
void cutBMP(const string& sourceFile, const string& newFile, int x, int y, int width, int height) {
	ofstream writeFile(newFile, ios::out | ios::binary);
	ifstream readFile(sourceFile, ios::in | ios::binary);
	if (!readFile) {
		cerr << "Error while opening the source BMP file!" << endl;
	}
	char BMPSourceFileHeader[54];
	readFile.read((char*)&BMPSourceFileHeader, 54);
	int sourceFileWidth = *(int*)&BMPSourceFileHeader[18];
	int sourceFileHeight = *(int*)&BMPSourceFileHeader[22];
	
	readFile.seekg(54+((y*sourceFileWidth)+x)*3);

	int filesize = 54 + 3 * width * height;
	char bmpfileheader[14] = { 'B','M', 0,0,0,0, 0,0,0,0, 54,0,0,0 };
	char exitBMPinfoHeader[40] = { 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0 };

	bmpfileheader[2] = (char)(filesize);
	bmpfileheader[3] = (char)(filesize >> 8);
	bmpfileheader[4] = (char)(filesize >> 16);
	bmpfileheader[5] = (char)(filesize >> 24);

	exitBMPinfoHeader[4] = (char)(width);
	exitBMPinfoHeader[5] = (char)(width >> 8);
	exitBMPinfoHeader[6] = (char)(width >> 16);
	exitBMPinfoHeader[7] = (char)(width >> 24);
	exitBMPinfoHeader[8] = (char)(height);
	exitBMPinfoHeader[9] = (char)(height >> 8);
	exitBMPinfoHeader[10] = (char)(height >> 16);
	exitBMPinfoHeader[11] = (char)(height >> 24);
	writeFile.write((const char*)bmpfileheader, 14);
	writeFile.write((const char*)exitBMPinfoHeader, 40);

	char currentPixel[3];
	for (int i = 0; i < height * 3; i++)
	{
		for (int j = 0; j < 3 * width; j += 3)
		{
			readFile.read((char*)& currentPixel, 3 * sizeof(char));
			writeFile.write((const char*)& currentPixel, 3 * sizeof(char));
		}

		readFile.seekg((sourceFileWidth - width) * 3 , ios::cur);
	}
}
string generateString(string line,int &count,char stop)
{
	string name="";
	while (true)
	{
		if (line[count] == stop)
		{
			count++;
			break;
		}
		else
		{
			name += line[count];
			count++;
		}
	}
	return name;
}
void readFromFile()
{
	string name,line;
	cout << "Enter file name:" << endl;
	cin >> name;
	ifstream readFile(name, ios::in);
	while (getline(readFile,line))
	{
		cout << line << endl;
		if (line[0] != ';')
		{
			string imageToCut="";
			string newImage="",cur="";
			int count = 0,x,y,width,height;
			imageToCut=generateString(line, count, '(');
			
			cur=generateString(line, count, ',');
			x = atoi(cur.c_str());
			cur = "";
			

			cur=generateString(line, count, ',');
			y = atoi(cur.c_str());
			cur = "";
			
			cur=generateString(line, count, ',');
			width = atoi(cur.c_str());
			cur = "";
			
			cur=generateString(line, count, ')');
			height = atoi(cur.c_str());
			cur = "";
			
			while (true)
			{
				if (count == line.length())
				{
					break;
				}
				else
				{
					newImage += line[count];
					count++;
				}
			}
			cutBMP(imageToCut, newImage, x, y, width, height);
		}
	}

}
int main()
{
	readFromFile();
	system("pause");
	return 0;

}
