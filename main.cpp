#include <stdio.h>
#include "vtkPolyData.h"
#include "vtkPolyDataReader.h"
#include "vtkPolyDataWriter.h"
#include <string>
#include <iostream>
#include <sstream>
#include "lv_func.h"
#include "vtkPoints.h"
#include "Vectors.h"
 
using namespace std;
int main(int argc ,char** argv)
{
	string Ivtkname=argv[1];
	string offsetname=argv[2];
	string TMname=argv[3];
	string Ovtkname=argv[4];

	vtkPolyDataReader* reader;
	reader=vtkPolyDataReader::New();
	vtkPolyData* data;
	data=vtkPolyData::New();
	vtkPolyDataWriter* writer;
	writer=vtkPolyDataWriter::New();

	reader->SetFileName(Ivtkname.c_str());
	reader->ReadAllColorScalarsOn();
	reader->ReadAllVectorsOn();
	reader->ReadAllScalarsOn();
	reader->ReadAllFieldsOn();
	reader->ReadAllNormalsOn();
	reader->ReadAllTCoordsOn();
	reader->ReadAllTensorsOn();
	reader->Update();
	data= reader ->GetOutput();
	
	float offset[3];
	fstream infile;
	infile.open(offsetname.c_str(), fstream::in);
	do{
		string line;
		getline(infile,line);
		string word;
		stringstream sin(line);
		sin>>word;
		if (word=="Offset")
		{
			sin>>word;
			for (int i=0;i<=2;i++)
			{
				sin>>offset[i];
			}
			break;			
		}

	}while(!infile.eof());
	infile.close();

	float T[4][4];
	infile.clear();
	infile.open(TMname,fstream::in);

	int lineid=0;
	for(string sline;getline(infile,sline);)
	{
		
		stringstream sin(sline);	
		for(int j=0;j<=3;j++)
		{
			sin>>T[lineid][j];
		}
      	lineid++;

	}

	lv_func fun;
	vtkPoints* points;
	points=vtkPoints::New();
	points->SetNumberOfPoints(data->GetNumberOfPoints());
	for (int i=0; i<=data->GetNumberOfPoints()-1;i++)
	{
		PointType ipoint,opoint;
		double coord[3];
		data->GetPoint(i,coord);
		for (int j=0;j<=2;j++)
		{
			ipoint.data[j]=coord[j];
		}
		fun.TransformVertex(ipoint,opoint,offset,T);
		points->SetPoint(i,opoint.data);
	}
	data->SetPoints(points);
	data->Update();

	writer->SetInput(data);
	writer->SetFileTypeToBinary();

	writer->SetFileName(Ovtkname.c_str());
	writer->Write();

	



	return (0);
}