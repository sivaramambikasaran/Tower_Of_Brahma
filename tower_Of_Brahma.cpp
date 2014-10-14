#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>
#include <fstream>
#include <sstream>

typedef std::vector<int> stick;

typedef std::vector<stick> configuration;

typedef std::vector<configuration> states;

//	This function moves discs from 'A' to 'C' using 'B'
void tower_Of_Brahma(int A, int C, int B, int n, states& allStates){
	if (n>0) {
		tower_Of_Brahma(A, B, C, n-1, allStates);
        configuration current   =   allStates.back();
        stick firstStick        =   current[A-1];
        stick secondStick       =   current[B-1];
        stick thirdStick        =   current[C-1];
        int discMoved      		=   firstStick.back();
        firstStick.pop_back();
        thirdStick.push_back(discMoved);
        configuration next;
        stick emptyStick;
        for (int k=0; k<3; ++k) {
            next.push_back(emptyStick);
        }
        next[A-1]   =   firstStick;
        next[B-1]   =   secondStick;
        next[C-1]   =   thirdStick;

        allStates.push_back(next);

		std::cout << std::endl << "Move disc from " << A << " to " << C << std::endl;

		tower_Of_Brahma(B, C, A, n-1, allStates);
	}
	return;
}

void assign_Disc_Radii(int N, double minRadius, double maxRadius, std::vector<double>& radii) {
    double radius   =   minRadius;
    radii.push_back(radius);
    double dR   =   (maxRadius-minRadius)/(N-1);
    for (int k=1; k<N; ++k) {
        radius+=dR;
        radii.push_back(radius);
    }
}

void draw(int N, states allStates, std::vector<double> radii, std::vector<double> center, double height, std::string fileName) {
    double radius;
	double stickThickness	=	0.25*radii[0];
	unsigned nDigits		=	ceil(log10(allStates.size()));
	unsigned nStart			=	1;
	for (unsigned k=0; k<nDigits; ++k) {
		nStart*=10;
	}
    for (int m=0; m<allStates.size(); ++m) {
		std::ostringstream oss;
		oss << nStart+m;
		std::string myFileName	=	fileName;
		myFileName+= oss.str();
		myFileName+=".tex";

		std::ofstream myfile;
		myfile.open(myFileName.c_str(), std::ios::out);

        configuration current   =   allStates[m];
		double diameter			=	center[2]-center[0];
		myfile << "\\documentclass[landscape]{standalone}" << std::endl;
		myfile << "\\usepackage{tikz}" << std::endl;
		myfile << "\\begin{document}" << std::endl;
        myfile << "\\begin{tikzpicture}[scale=0.5]" << std::endl;
		//	Draw the baseline
		myfile << "\\draw [draw = white, fill=white] (" << 1.5*center[0] << "," << -0.5*height << ") rectangle (" << 1.5*center[2] << "," << (N+1.5)*height << ");" << std::endl;
		myfile << "\\draw (" << 1.5*center[0] << ", " << -0.5*height << ") -- (" << 1.5*center[2] << ", " << -0.5*height << ");" << std::endl;
		//	Draw the sticks
		for (int l=0; l<3; ++l) {
			myfile << "\\draw [fill = black] (" << center[l]-stickThickness << ", -" << height << ") rectangle (" << center[l]+stickThickness << ", " << (N+1.5)*height << ");" << std::endl;
		}
        for (int l=0; l<3; ++l) {
            for (int k=0; k<current[l].size(); ++k) {
                radius  =   radii[current[l][k]];
                myfile << "\\draw [fill = blue] (";
                myfile << center[l] - radius << ", " << (k-0.5)*height << ") rectangle ";
                myfile << "(" << center[l] + radius << ", " << (k+0.5)*height << ");";
                myfile << std::endl;
            }
        }
        myfile << "\\end{tikzpicture}" << std::endl;
		myfile << "\\end{document}" << std::endl;
		myfile.close();
    }
}

void print_Each_Step(states allStates) {
    int nSteps =   allStates.size();
    for (int k=0; k<nSteps; ++k) {
        std::cout << "After " << k << " steps: " << std::endl;
        for (int m=0; m<3; ++m) {
            std::cout << "Stick " << m+1 << " contains: " << std::endl << "( ";
            for (int l=0; l<allStates[k][m].size(); ++l) {
                std::cout << allStates[k][m][l] << " ";
            }
            std::cout << ");" << std::endl;
        }
    }
	std::cout << std::endl;
}

void write_Script_File(unsigned N) {
	std::ofstream myfile;
	int nMoves	=	1;
	for (unsigned k=0; k<N; ++k) {
		nMoves*=2;
	}
	--nMoves;
	unsigned nDigits		=	ceil(log10(nMoves));
	unsigned nStart			=	1;
	for (unsigned k=0; k<nDigits; ++k) {
		nStart*=10;
	}
	myfile.open("script.sh", std::ios::out);
	myfile << "#!/bin/bash" << std::endl;
	myfile << "cd images/" << std::endl;
	myfile << "for i in `seq " << nStart << " " << nStart+nMoves<<" `;" << std::endl;
	myfile << "do" << std::endl;
	myfile << "\t pdflatex drawing$i.tex" << std::endl;
	myfile << "\t convert drawing$i.pdf drawing$i.gif" << std::endl;
	myfile << "\t rm drawing$i.aux drawing$i.log" << std::endl;
	myfile << "done" << std::endl;
	myfile << "gifsicle --delay=100 --loop drawing*.gif > ./../animationTowerOfBrahma.gif" << std::endl;
	myfile << "cd ./../" << std::endl;
	myfile.close();
}

// void make_Movie() {
// 	std::ofstream myfile;
// 	myfile.open("movie.tex");
// 	myfile << "\\documentclass{standalone}" << std::endl;
// 	myfile << "\\usepackage{animate}" << std::endl;
// 	myfile << "\\begin{document}" << std::endl;
// 	myfile << "\\begin{frame}" << std::endl << std::endl;
// 	myfile << "\\animategraphics[controls,scale=1]{2}{./finished}{}{}" << std::endl << std::endl;
// 	myfile << "\\end{frame}" << std::endl;
// 	myfile << "\\end{document}" << std::endl;
// }

int main(int argc, char* argv[]) {
	int N;
	std::cout << "Enter the number of discs: " << std::endl;
	std::cin >> N;
    //  Initialize the first stick.
    stick firstStick;
    for (int k=N-1; k>-1; --k) {
        firstStick.push_back(k);
    }
    stick secondStick;
    stick thirdStick;

    configuration initial;
    initial.push_back(firstStick);
    initial.push_back(secondStick);
    initial.push_back(thirdStick);

    states allStates;
    allStates.push_back(initial);

	//	Does the tower of Brahma.
	tower_Of_Brahma(1,3,2,N,allStates);

	//	Print all the steps on terminal.
	print_Each_Step(allStates);
    
	// //	Make the movie file.
	// make_Movie();

    double minRadius    =   1.0;
    double maxRadius    =   5.0;
    std::vector<double> radii;
    assign_Disc_Radii(N, minRadius, maxRadius, radii);

    std::vector<double> stickCenter;
    stickCenter.push_back(-2.5*maxRadius);
    stickCenter.push_back(0.0);
    stickCenter.push_back(2.5*maxRadius);

    double height       	=	minRadius;
    std::string fileName    =   "./images/drawing";
    draw(N, allStates, radii, stickCenter, height, fileName);
	write_Script_File(N);
}