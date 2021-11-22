#include "Chemical.h"
using namespace std;

/*
Eren Karakas
22002722
*/

class Cabinet {
public:
    Cabinet();
    Cabinet( int id, int rows, int columns );
    ~Cabinet();
    
    // Getters
    int getMaxRow();
    int getMaxColumn();
    int getRowNum();
    int getColumnNum();
    int getSize();
    int getID();
    int getEmptyPlaces();
    void getContents();
    Chemical getItem( int row, int column );

    // Setters
    void setRowNum( int );
    void setColumnNum( int );
    void setID( int );
    void setEmptyPlaces( int );

    // Other necessary methods
    bool checkSurroundings( int, int );
    bool putChemical( int row, int column, Chemical chem );
    string findClosest( int, int );
    string findChem( int id );
    void removeChem( int id );
    Cabinet& operator=( Cabinet& right );
    
private:
    static const int MAX_ROW = 9;
    static const int MAX_COLUMN = 9;
    string ROW_NAMES[9];
    int rowNum;
    int columnNum;
    int id;
    int emptyPlaces;
    Chemical** cabinetArray;
};