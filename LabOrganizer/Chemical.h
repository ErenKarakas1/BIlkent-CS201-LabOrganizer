using namespace std;

/*
Eren Karakas
22002722
*/

class Chemical {
public:
    Chemical();
    Chemical( int id, string type );
    
    int getID();
    string getType();
    void setID( int id );
    void setType( string type );
    Chemical& operator=( Chemical& right );
    
private:
    int id;
    string type;
};