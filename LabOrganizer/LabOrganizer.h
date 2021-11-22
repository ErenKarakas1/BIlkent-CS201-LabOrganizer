#include "Cabinet.h"
using namespace std;

/*
Eren Karakas
22002722
*/

class LabOrganizer {
public:
    LabOrganizer();
    ~LabOrganizer();
    
    void addCabinet(int id, int rows, int columns);
    void removeCabinet(int id);
    void listCabinets();
    void cabinetContents(int id);
    void placeChemical(int cabinetId, string location, string chemType, int chemID);
    void findChemical(int id);
    void removeChemical(int id);
    
private:
    static const int MAX_ROW = 9;
    static const int MAX_COLUMN = 9;
    Cabinet* cabinets;
    int size;
};