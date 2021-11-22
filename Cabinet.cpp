#include <iostream>
#include <sstream>
#include <cmath>
using namespace std;
#include "Cabinet.h"

/*
Eren Karakas
22002722
*/

Cabinet::Cabinet() {

    // Extended initializer lists not supported in djikstra
    ROW_NAMES[0] = "A";
    ROW_NAMES[1] = "B";
    ROW_NAMES[2] = "C";
    ROW_NAMES[3] = "D";
    ROW_NAMES[4] = "E";
    ROW_NAMES[5] = "F";
    ROW_NAMES[6] = "G";
    ROW_NAMES[7] = "H";
    ROW_NAMES[8] = "I";

    cabinetArray = NULL;
    id = 0;
    rowNum = 0;
    columnNum = 0;
}

Cabinet::Cabinet( int id, int rows, int columns ) {

    // Extended initializer lists not supported in djikstra
    ROW_NAMES[0] = "A";
    ROW_NAMES[1] = "B";
    ROW_NAMES[2] = "C";
    ROW_NAMES[3] = "D";
    ROW_NAMES[4] = "E";
    ROW_NAMES[5] = "F";
    ROW_NAMES[6] = "G";
    ROW_NAMES[7] = "H";
    ROW_NAMES[8] = "I";

    cabinetArray = new Chemical* [rows];

    for ( int i = 0; i < rows; ++i ) {
        cabinetArray[i] = new Chemical[columns];
    }

    this->id = id;
    rowNum = rows;
    columnNum = columns;
    emptyPlaces = rowNum * columnNum;
}

Cabinet::~Cabinet() {
    // Delete the 2D array and remove the dangling pointer, nullptr not supported
    if ( cabinetArray == NULL ) {
        return;
    }

    for (int i = 0; i < rowNum; i++) {
        delete [] cabinetArray[i];
    }
    delete [] cabinetArray;
    cabinetArray = NULL;
}

int Cabinet::getMaxRow() {
    return MAX_ROW;
}

int Cabinet::getMaxColumn() {
    return MAX_COLUMN;
}

int Cabinet::getRowNum() {
    return rowNum;
}

int Cabinet::getColumnNum() {
    return columnNum;
}

int Cabinet::getSize() {
    return rowNum * columnNum;
}

int Cabinet::getID() {
    return id;
}

int Cabinet::getEmptyPlaces() {
    return emptyPlaces;
}

void Cabinet::getContents() {
    string* outRow = new string[rowNum];
    string out = "   ";
    string chemicals;
    string chemID;
    stringstream ss2;
    stringstream ss3;

    for (int i = 0; i < columnNum; i++)
    {
        // Using stringstream since to_string() is not supportedin djikstra
        stringstream ss;
        string num;
        ss << ( i + 1 );
        ss >> num;
        out += num + " " ;
    }
    out = out + "\n";

    for (int i = 0; i < rowNum; i++ ) {
        for (int j = 0; j < columnNum; j++) {
            outRow[i] += " " + cabinetArray[i][j].getType();
            
            if ( cabinetArray[i][j].getType() != "+" ) {
                // Magic number used for ASCII convention
                char second = j + 49;
                string location = ROW_NAMES[i] + second;
                
                stringstream ss1;
                ss1 << cabinetArray[i][j].getID();
                ss1 >> chemID;
                
                chemicals += location + ": " + chemID + ", ";
            }
        }
        outRow[i] += "\n";        
    }
    
    for (int i = 0; i < rowNum; i++) {
        out += ROW_NAMES[i] + " " + outRow[i];
    }

    string row;
    string column;

    ss2 << rowNum;
    ss2 >> row;
            
    ss3 << columnNum;
    ss3 >> column;

    string dimensions = row + "x" + column;
    chemicals = chemicals.substr( 0, chemicals.length() - 2 ); // Remove the last comma
    
    cout << "ID: " << id << ", " << dimensions << ", empty: " << emptyPlaces << ". Chemicals: " << chemicals << endl;
    cout << out;

    delete[] outRow;
}

Chemical Cabinet::getItem( int row, int column ) {
    return cabinetArray[row][column];
}

void Cabinet::setRowNum( int rows ) {
    rowNum = rows;
}

void Cabinet::setColumnNum( int columns ) {
    columnNum = columns;
}

void Cabinet::setID( int newID ) {
    id = newID;
}

void Cabinet::setEmptyPlaces( int emptyPlaces ) {
    this->emptyPlaces = emptyPlaces;
}

bool Cabinet::checkSurroundings( int row, int column ){
    /*
    Checks all 8 squares around the point
    Checks if up, left, down, right are valid then checks diagonals if they are
    */

    bool right = false, left = false, up = false, down = false;

    if( row - 1 >= 0 ) {
        if( cabinetArray[row-1][column].getType() == "c" ) {
            return false;

        }
        up = true;
    }

    if ( row + 1 < rowNum ) {
        if( cabinetArray[row+1][column].getType() == "c" ) {
            return false;
        }
        down = true;
    }

    if ( column + 1 < columnNum) {
        if ( cabinetArray[row][column+1].getType() == "c") {
            return false;
        }
        right = true;
        
    }

    if ( column - 1 >= 0 ) {
        if ( cabinetArray[row][column-1].getType() == "c") {
            return false;
        }
        left = true;
    }

    if( right && up ) {
        if ( cabinetArray[row-1][column+1].getType() == "c") {
            return false;
        }
    }

    if( right && down ) {
        if ( cabinetArray[row+1][column+1].getType() == "c") {
            return false;
        }
    }

    if( left && up ) {
        if ( cabinetArray[row-1][column-1].getType() == "c") {
            return false;
        }
    }

    if( left && down ) {
        if ( cabinetArray[row+1][column-1].getType() == "c") {
            return false;
        }
    }

    return true;
}

bool Cabinet::putChemical( int row, int column, Chemical chem ) {

    if ( chem.getType() == "r" ) {
        cabinetArray[row][column].setID( chem.getID() );
        cabinetArray[row][column].setType( chem.getType() );
        emptyPlaces--;
        return true;
    }
    else if ( chem.getType() == "c") {
        if ( checkSurroundings( row, column ) ) {
            cabinetArray[row][column].setID( chem.getID() );
            cabinetArray[row][column].setType( chem.getType() );
            emptyPlaces--;
            return true;
        }
    }

    return false;
}

string Cabinet::findClosest( int row, int column ) {
    string final = "";
    int count = 0;
    int letterIndex;
    int numIndex;
    int smallest;


    for (int i = 0; i < rowNum; i++) {
        for (int j = 0; j < columnNum; j++) {
            if( i != row || j != column ){
                
                if( checkSurroundings( i, j ) && getItem( i, j ).getType() == "+") {
                    count++;
                }
            }
        }
    }    

    if( count == 0 ){
        final = "No available spots were found.";
        return final;
    }
    

    string* locations = new string[count];
    int* candidates = new int[count];
    
    count = 0;
    
    for ( int i = 0; i < rowNum; i++ ) {
        for ( int j = 0; j < columnNum; j++ ) {
            if( i != row || j != column ) {

                if( checkSurroundings( i, j ) && getItem( i, j ).getType() == "+"){
                    stringstream ss;
                    string num;
                    ss << ( j + 1 );
                    ss >> num;
                    
                    string location = "";
                    location += ROW_NAMES[i];
                    location += num;

                    locations[count] = location;
                    count++;
                }
            }
        }
    }    


    for (int i = 0; i < count; i++) {
        for (int j = 0; j < 9; j++) {
            if( ROW_NAMES[j] == locations[i].substr(0,1)){
                letterIndex = j;
            }
        }

        // Magic number used for ASCII convention
        numIndex = locations[i][1] - 49;
        
        if( abs( letterIndex - row ) >= abs( numIndex - column )){
            candidates[i] = abs( letterIndex - row );
        }
        else {
            candidates[i] = abs( numIndex - column );
        }
    }
    
    // Find smallest possible distance

    smallest = candidates[0];

    for (int i = 0; i < count; i++) {
        if( smallest > candidates[i] ){
            smallest = candidates[i];
        }
    }

    for (int i = 0; i < count; i++) {
        if( candidates[i] == smallest){
            final += locations[i] + ", ";
        }
    }

    // Finalize string

    final = final.substr( 0, final.length() - 2 );
    delete [] locations;
    delete [] candidates;

    return final;
}

string Cabinet::findChem( int id ) {
    string location = "00";
    
    for ( int i = 0; i < rowNum; i++ ) {
        for ( int k = 0; k < columnNum; k++ ) {
            if ( cabinetArray[i][k].getID() == id ) {

                // Magic number used for ASCII convention
                char second = k + 49;
                location = ROW_NAMES[i] + second;
            }
        }
    }

    return location;
}

void Cabinet::removeChem( int id ) {
    for ( int i = 0; i < rowNum; i++ ) {
        for ( int k = 0; k < columnNum; k++ ) {
            if ( cabinetArray[i][k].getID() == id ) {
                cabinetArray[i][k].setID( 0 );
                cabinetArray[i][k].setType( "+" );
            }
        }
    }
}

Cabinet& Cabinet::operator=( Cabinet& right ) {
    if( &right != this ) {

        this->setRowNum( right.getRowNum() );
        this->setColumnNum( right.getColumnNum() );
        this->setID( right.getID() );
        this->setEmptyPlaces( right.getEmptyPlaces() );
        
        cabinetArray = new Chemical*[rowNum];

        for (int i = 0; i < rowNum; i++) {
            cabinetArray[i] = new Chemical[columnNum]; 
        }

        for (int i = 0; i < right.getRowNum(); i++) {
            for (int j = 0; j < right.getColumnNum(); j++) {
                int id = right.cabinetArray[i][j].getID();
                string type = right.cabinetArray[i][j].getType();
                
                cabinetArray[i][j].setID( id );
                cabinetArray[i][j].setType( type );
            }   
        }
    }

    return *this;
}