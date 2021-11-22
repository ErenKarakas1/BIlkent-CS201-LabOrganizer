#include <iostream>
#include <sstream>
using namespace std;
#include "LabOrganizer.h"

/*
Eren Karakas
22002722
*/

LabOrganizer::LabOrganizer() {
    cabinets = NULL;
    size = 0;
}

LabOrganizer::~LabOrganizer() {
    delete [] cabinets;
}
    
void LabOrganizer::addCabinet(int id, int rows, int columns) {
    if ( rows > MAX_ROW || columns > MAX_COLUMN ) {
        cout << "Cannot add the cabinet: Dimensions are out of bounds." << endl;
        return;
    }

    if ( cabinets == NULL ) {
        cabinets = new Cabinet[1];
        Cabinet* cabinet = new Cabinet( id, rows, columns );
        cabinets[0] = *cabinet;
        
        size++;
        cout << "Added a cabinet: ID " << id << " and dimensions " << rows << " to " << columns << endl;
        
        delete cabinet;
        return;
    }

    for ( int i = 0; i < size; ++i ) {
        if ( cabinets[i].getID() == id ) {
            cout << "Cannot add the cabinet: ID " << id << " already in the system." << endl; 
            return;
        }
    }
    
    Cabinet* temp = cabinets;
    cabinets = new Cabinet[size+1];

    for ( int i = 0; i < size; i++ ) {
        cabinets[i] = temp[i];
    }

    Cabinet* cabinet = new Cabinet( id, rows, columns );
    cabinets[size] = *cabinet;

    cout << "Added a cabinet: ID " << id << " and dimensions " << rows << " to " << columns << endl;

    delete [] temp;
    delete cabinet;
    size++;
}

void LabOrganizer::removeCabinet(int id) {
    bool cabinetExists = false;
    int index;

    for ( int i = 0; i < size; i++ ) {
        if ( cabinets[i].getID() == id ) {
            for ( int k = 0; k < cabinets[i].getRowNum(); k++ ) {
                for ( int j = 0; j < cabinets[i].getColumnNum(); j++ ) {
                    if ( cabinets[i].getItem( k, j ).getType() != "+" ) {
                        removeChemical( cabinets[i].getItem( k, j ).getID() );
                    }
                }
            }

            cabinetExists = true;
            index = i;
        }
    }

    if ( !cabinetExists ) {
        cout << "Cabinet " << id << " does not exist in the system." << endl;
        return;
    }


    Cabinet* temp = cabinets;
    cabinets = new Cabinet[size-1];

    for ( int k = 0; k < index; k++ ) {
        cabinets[k] = temp[k];
    }

    for ( int l = index+1; l < size; l++ ) {
        cabinets[l-1] = temp[l];
    }

    cout << "Cabinet " << id << " has been removed." << endl;

    delete [] temp;
    size--;
}

void LabOrganizer::listCabinets() {
    if ( size == 0 ) {
        return;
    }
    else {
        cout << "List of all cabinets: " << endl;
        for ( int i = 0; i < size; ++i ) {
            int id = cabinets[i].getID();
            int rows = cabinets[i].getRowNum();
            int columns = cabinets[i].getColumnNum();
            
            // Using stringstream since to_string() does not work in djikstra
            string row;
            string column;
            stringstream ss;

            ss << rows;
            ss >> row;
            
            stringstream ss1;
            ss1 << columns;
            ss1 >> column;

            string dimensions = row + "x" + column;
            int emptySlots = cabinets[i].getEmptyPlaces();

            cout << "ID: " << id << ", Dim: " << dimensions << ", Number of empty slots: " << emptySlots << endl;
        }
    }
}

void LabOrganizer::cabinetContents(int id) {
    bool cabinetExists = false;

    for ( int i = 0; i < size; ++i ) {
        if ( cabinets[i].getID() == id ) {
            cabinets[i].getContents();

            cabinetExists = true;
            break;
        }
    }

    if ( !cabinetExists ) {
        cout<< "Cabinet " << id << " is not in the system." << endl;
    }
}

void LabOrganizer::placeChemical(int cabinetId, string location, string chemType, int chemID) {

    // Extended initializer lists not supported in djikstra
    string ROW_NAMES[9];
    ROW_NAMES[0] = "A";
    ROW_NAMES[1] = "B";
    ROW_NAMES[2] = "C";
    ROW_NAMES[3] = "D";
    ROW_NAMES[4] = "E";
    ROW_NAMES[5] = "F";
    ROW_NAMES[6] = "G";
    ROW_NAMES[7] = "H";
    ROW_NAMES[8] = "I";

    bool cabinetExists = false;


    for ( int i = 0; i < size; i++ ) {
        if ( cabinets[i].findChem( chemID ) != "00" ) {
            cout << "Chemical with ID " << chemID << " already exists in the system." << endl;
            return;
        }
    }

    for ( int i = 0; i < size; ++i ) {
        if ( cabinets[i].getID() == cabinetId ) {
            
            cabinetExists = true;

            Chemical* chem = new Chemical( chemID, chemType.substr(0,1) );

            int row;

            for (int i = 0; i < 9; i++) {
                if( ROW_NAMES[i] == location.substr(0,1) ) {
                    row = i;
                }
            }

            int column = location[1] - 49;

            if ( cabinets[i].getItem( row, column ).getType() != "+" ) {
                cout << "Location " << location << " in cabinet " << cabinetId 
                     << " is already occupied." << endl;
                
                delete chem;
                return;
            }

            bool didPut = cabinets[i].putChemical( row, column, *chem );
            delete chem;

            if ( didPut ) {
                // Magic number used for ASCII code convention
                chemType[0] = chemType[0] - 32;

                cout << chemType << " chemical with ID " << chemID << " has been placed at location " 
                    << location << " in cabinet " << cabinetId << "." << endl;
            }
            else {
                cout << "Location " << location << " in cabinet " << cabinetId 
                     << " is not suited for a combustive chemical. Nearest possible locations for this chemical: "
                     << cabinets[i].findClosest( row, column ) << endl;
            }

            break;
        }
    }

    if ( !cabinetExists ) {
        cout << "Cabinet does not exist." << endl;
        return;
    }
}

void LabOrganizer::findChemical( int id ) {
    bool found = false;
    int cabinetID;
    string chemLocation;

    for ( int i = 0; i < size; ++i ) {
        string location = cabinets[i].findChem( id );

        if ( location != "00" ) {
            cabinetID = cabinets[i].getID();
            chemLocation = location;
            
            found = true;
            break;
        }
    }

    if ( found ) {
        cout << "Chemical " << id << " is at location " << chemLocation << " in cabinet " << cabinetID << endl;
    }
    else {
        cout << "Chemical " << id << " is not in the system." << endl;
    }
}

void LabOrganizer::removeChemical(int id) {
    bool found = false;
    int cabinetID;
    int index;

    for ( int i = 0; i < size; ++i ) {
        string location = cabinets[i].findChem( id );

        if ( location != "00" ) {
            cabinetID = cabinets[i].getID();
            index = i;

            found = true;
            break;
        }
    }

    if ( found ) {
        cabinets[index].removeChem( id );
        cout << "Chemical " << id << " removed from cabinet " << cabinetID << endl;
    }
    else {
        cout << "Chemical " << id << " is not in the system." << endl;
    }
}