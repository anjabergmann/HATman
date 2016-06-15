%module arianne

%include "std_string.i"
%include "std_vector.i"
%include "std_list.i"
%include "std_map.i"

%{
#include "SDL.h"
#include "libFramework.h"
%}

// Grab a Python function object as a Python object.
%typemap(python,in) PyObject *pyfunc 
  {
  if (!PyCallable_Check($input)) 
    {
    PyErr_SetString(PyExc_TypeError, "Need a callable object!");
    return NULL;
    }
  $1 = $input;
  }

%{
/* This function matches the prototype of the normal C callback
   function for our widget. However, we use the clientdata pointer
   for holding a reference to a Python callable object. */
static bool PythonCallBack(void *clientdata)
  {
  PyObject *func, *arglist;
  PyObject *result;
   
  func = (PyObject *) clientdata;               // Get Python function
  arglist = Py_BuildValue("()");                // Build argument list
  result = PyEval_CallObject(func,arglist);     // Call Python
  Py_DECREF(arglist);                           // Trash arglist
  int dres=1;
  if (result) 
    {                                
    dres=PyInt_AsLong(result);
    }
  Py_XDECREF(result);   
  return dres!=0;  
  }

void setIdleMethod(PyObject *pyfunc) 
  {
  callback=PythonCallBack;
  pythonCallbackFunction=pyfunc;
  Py_INCREF(pyfunc);
  }
%}

void setIdleMethod(PyObject *pyfunc);



typedef unsigned int Uint32;
typedef unsigned char Uint8;

namespace std
  {
  %template(stdListString) list<string>;
  %template(stdVectorUint8) vector<Uint8>;
  %template(stdListRPObject) list<arianne::RPObject>;
  %template(stdMapUint32RPObject) map<Uint32,arianne::RPObject>;
  }
  
namespace arianne
  {
  class RPSlot; 
  
  class RPAction
    {
    public:
    RPAction();
    ~RPAction();
    
	bool has(std::string const& attribute) const;	
	void put(std::string const& attribute, std::string const& value);
	void put(std::string const& attribute, Uint32 value);	
	std::string get(std::string const& attribute) const;
	Uint32 getInt(std::string const& attribute) const;
	void remove(std::string const& attribute);
	void clear();	
    
    bool equals(arianne::RPAction const& b) const;
    std::string toString() const;
    Uint32 size();    
    };
    
  class RPObject
    {
    public:
    RPObject();
    ~RPObject();
    void addSlot(RPSlot& slot);
    RPSlot* getSlot(std::string const& name);
    void removeSlot(std::string const& name);
    bool hasSlot(std::string const& name) const;    

	bool has(std::string const& attribute) const;	
	void put(std::string const& attribute, std::string const& value);
	void put(std::string const& attribute, Uint32 value);	
	std::string get(std::string const& attribute) const;
	Uint32 getInt(std::string const& attribute) const;
	void remove(std::string const& attribute);
	void clear();	
    
    bool equals(arianne::RPObject const& b) const;
    std::string toString() const;
    Uint32 size();    

    void applyAddedDifferences(RPObject added);
    void applyDeletedDifferences(RPObject deleted);
    void removeAllHidden();
    };
  
  class RPSlot
    {
    public:
    std::string name;    

    RPSlot();
    ~RPSlot();

    void add(RPObject const& object);
    RPObject& get(Uint32 id);
    RPObject& getByPosition(Uint32 i);
    bool has(Uint32 id) const;
    void remove(Uint32 id);
    void clear();

    bool equals(arianne::RPSlot const& b) const;
    std::string toString() const;
    Uint32 size() const;        
    };
  }

struct Perception 
  {
  static const Uint8 SYNC=1;
  static const Uint8 DELTA=0;
  
  Uint8 type;
  Uint32 timestamp;
  std::list<arianne::RPObject> added;
  std::list<arianne::RPObject> modifiedAdded;
  std::list<arianne::RPObject> modifiedDeleted;
  std::list<arianne::RPObject> deleted;
  
  bool myRPObjectChanged;
  arianne::RPObject myRPObject;
  };

struct World
  {
  std::map<Uint32,arianne::RPObject> objects;  
  };  

 
void connectToArianne(char *serverName, int port);
bool login(char *username, char *password);
std::list<std::string> availableCharacters();
std::list<std::string> serverInfo();
std::string getGameType();
bool chooseCharacter(char *character);
Uint32 ourRPObjectID();

bool hasRPMap();
std::list<arianne::RPObject> getRPMap();

bool hasPerception();
Perception getPerception();

void send(arianne::RPAction action);
Uint32 sendBlocking(arianne::RPAction action);

void outOfSync();

bool logout();

bool iserror();
Uint32 errorCode();
std::string errorReason();