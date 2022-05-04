#include <cstring>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

typedef int location; 

const int number_of_classrooms = 18;
const int number_of_floors = 3;
const int number_per_floor = 6;

struct classroom_id {
    int floor;
    int number;

    void print() {
        cout << floor << "." << number;
    }
};

struct name {
    string firstName;
    string lastName;

    void print() {
        cout << firstName << " " << lastName;
    }
};

class PEOPLE{
protected:
    int ftg; //fatigue 
    name nam; //name
    classroom_id id;
    bool inside; //flag whether they are inside or not 
public:
    
    PEOPLE():ftg(0){
        id.floor= 0;
        id.number =0;
        inside = false;        
    }

    void namePerson(string a){
        nam.lastName = a;
    }

    void print() {
        nam.print();        
        cout << "\t";
    }

    virtual void printWithFtg(){
        print();
        cout << "Fatigue: " << ftg << endl;
    }

    virtual void attend(){
        
    }

    virtual void teach(){

    }

    void place(){
        inside = true;
    }

    bool is_inside() {
        return inside;
    }  

    void assignClassroom(classroom_id  cl) {
        id = cl;
    }

    virtual ~PEOPLE(){
        
    }
};

class TEACHER : public PEOPLE{
    int points; //points of fatigue every hour
public:
    TEACHER(int p):points(p) {
        nam.firstName = "Teacher";
        nam.lastName = "Default";        
        inside = false;
    }            

    int getTP(){
        return points;
    }

    void printC() {
        print();
        id.print();
        cout << "\n";
    }              

    void teach(){
        ftg += points;
        
    }

    ~TEACHER() {
        cout << "A Teacher to be destroyed : ";
        printC();

    }
};

class STUDENT : public PEOPLE{
    location loc;
public:
    STUDENT():loc(0) {
        nam.firstName = "Student";
        nam.lastName = "Default";        
    }

    STUDENT(string str) :loc(0) {
        nam.firstName = "Student";
        nam.lastName = str;
    }  

    virtual void jf(){
        cout << "but why";
    }

    void assignLocation(const location x) {
        this->loc = x;
    }

    int getFloorID()const {
        return id.floor;
    }

    virtual void attend(){
        cout << "virtual attend" << endl;
        ftg += 0;
    }     

    int getNumberID()const {
        return id.number;
    }   

    int getFtg(){
        return ftg;
    } 

    void printT() {
        nam.print();
        cout << " ";
    }

    void printC() {
        print();
        cout << " ";
        id.print();
        cout << "\n";
    }       

    ~STUDENT() {
        cout << "A Student to be deleted : ";
        printC();
        cout << "and their location is ";
        if (loc == 0) {
            cout << "School building\n";
        }
        else if (loc == 1) {
            cout << "Yard\n";
        }
        else if (loc == 2) {
            cout << "Stairs\n";
        }
        else if (loc == 3) {
            cout << "Floor " << id.floor << " corridor\n";
        }
        else if (loc == 5) {
            cout << "Classroom "; id.print();
            cout << endl;
        }
    }
};

class JUNIOR: public STUDENT{
    int points;
public:   

    JUNIOR(int p):points(p){
    }    

    void attend(){
        ftg += points;
    }   
};

class SENIOR: public STUDENT{
    int points;
public:
    SENIOR(int p):points(p){

    }  

    void attend(){
        ftg += points;
    }   
};

class LOCATION{
protected:
    location loc;
    int current_number;
public:
    LOCATION(location l){
        loc = l;
        current_number = 0;
    } 

    virtual int enter(STUDENT* s){
        cout << "Location does not support entry" << endl;
        return -2;
    }
    
    virtual STUDENT* exit(){
        cout << "Location does not allow a student to be exited\n";
        return nullptr;
    }

    virtual ~LOCATION(){

    };
        
};

class CLASSROOM : public LOCATION{
protected:
    const location classroom = 5;
    TEACHER *t;
    STUDENT** s;
    classroom_id id;
    int capacity;

public:
    
    static int j;

    CLASSROOM(int max_c) :LOCATION(classroom),capacity(max_c) {
        id.floor = 0;
        id.number = 0;
    }

    void assignClassId(int floor, int number) {
        id.floor = floor;
        id.number = number;
    }    

    int getNo() {
        return current_number;
    }    

    classroom_id getID() {
        return id;
    }   

    bool getInside() {
        return t->is_inside();
    }   

    void nameTeacher(string a) {
        t->namePerson(a);
        cout << endl;
    }

    TEACHER* getTeacher() {
        return t;
    }

    void assignTeacher() {
        t->assignClassroom(id);
    }

    virtual void assignStudent(STUDENT& s) {
        s.assignClassroom(id);
    }

    void printID() {
        id.print();
    }

    void printStudents() {
        cout << "Teacher is : " ;
        t->printWithFtg();
        for (int i = 0; i < current_number; i++) {
            s[i]->printWithFtg();
            cout << "\n";
        }
    }

    virtual void operate(int h)=0;

    

    void printTeacher() {
        cout << "The teacher is: ";
        t->print();
        cout << endl;
    }


    int enter(STUDENT* st) { //GO TO CORRIDOR CLASS TO SEE INFO 
            st->assignLocation(classroom); //assigns location to student 
            st->place();
            s[current_number] = st; //
            current_number++;
            st->printT();
            cout << "enters classroom\n";
            
            return 0;
        
        
    }

    virtual ~CLASSROOM() {
        delete t;
        for (int i = 0; i < current_number; i++) {
            s[i] = nullptr;
            delete s[i];
        }
        delete[]s;
    }

    
};

int CLASSROOM ::j = 0; //init of static class member

class SENIOR_CLASS: public CLASSROOM{
    int points;
    
public:

    SENIOR_CLASS(int max_c, int tp , int p, STUDENT **st):CLASSROOM(max_c), points(p){
        s = new STUDENT * [capacity];
        t = new TEACHER(tp);
        
        for (int i = 0; i < capacity; i++) {
                s[i] = new SENIOR(points);
                st[j] = new SENIOR(points); // creates the 
                j++;
                s[i] = nullptr;
            }
    }

    void operate(int h){        
        for(int i = 0 ; i < current_number ; i++){
            for(int j =0 ; j < h; j++){
                s[i]->attend();                
            }
        }
        for(int j =0 ; j < h; j++){
            t->teach();
        }
          
    }

    void assignStudent(SENIOR& s) {
        s.assignClassroom(id);
    }

    ~SENIOR_CLASS(){
         
    }
};

class JUNIOR_CLASS: public CLASSROOM{
    int points;
public:
    JUNIOR_CLASS(int max_c,int tp, int p, STUDENT **st):CLASSROOM(max_c), points(p){
        s = new STUDENT * [capacity];
        t = new TEACHER(tp);
         for (int i = 0; i < capacity; i++) {
                s[i] = new JUNIOR(points);
                st[j] = new JUNIOR(points);
                j++;
                s[i] = nullptr;
            }
    }

    void assignStudent(JUNIOR& s) {
        s.assignClassroom(id);
    }

    void operate(int h){        
        for(int i = 0 ; i < current_number ; i++){
            for(int j =0 ; j < h; j++){
                s[i]->attend();
            }
        }
        for(int j =0 ; j < h; j++){
            t->teach();
        }        
    }

    ~JUNIOR_CLASS(){
        
    }
};

class YARD :public LOCATION{
    const location yard = 1;
    STUDENT** s;
public:
    YARD() : LOCATION(yard){
        s = new STUDENT *;
        *s = new STUDENT;
        *s = nullptr;
        
        cout << "The Yard has been created! "<<endl; 
    }   

    void printStudents() { // copied from 1st ex , for loop and array traversal is not mandatory , but it does not pose a problem
        for (int i = 0; i < current_number; i++) { // and it can easily be converted later 
            s[i]->printWithFtg();
            cout << "\n";
        }
    }

    int enter(STUDENT* st) {
        st->assignLocation(yard);
            st->printT();
            cout << "enters yard\n";
            *s = st;
            current_number++;
            
            return 0;      
    }

    int getNumber() {
        return current_number;
    }

    STUDENT* exit() {//GO TO CORRIDOR CLASS FOR INFO
        if (*s != nullptr) {
            STUDENT* temp = *s;
            current_number--;
            (*s)->printT();
            cout << "exits yard\n";
            *s = nullptr;             
            return temp;
        }
        else {
            cout << "Could not exit student : there are no students to be exited from yard\n";
            return nullptr;
        }
    }

    virtual ~YARD() {        
        *s = nullptr;
        delete *s;        
        delete[]s;
    }
};

class STAIRS:public LOCATION {    
    const location stairs = 2;
    STUDENT** s;

public:
    STAIRS():LOCATION(stairs){
        current_number= 0;
        s = new STUDENT * ;        
        *s = new STUDENT;
        *s = nullptr;        
        cout << "The stairs have been created\n";
    }   

    int getNumber() {
        return current_number;
    }       

    int enter(STUDENT* st) {       
        st->assignLocation(stairs);
        *s = st;
        current_number++;
        st->printT();
        cout << "enters stairs\n";            
        return 0;        
    }

    void printStudents() {
        for (int i = 0; i < current_number; i++) {
            s[i]->printWithFtg();
            cout << "\n";
        }
    }

    STUDENT* exit() {
        if (*s != nullptr) {
            STUDENT* temp = *s;
            current_number--;
            (*s)->printT();
            cout << "exits stairs\n";
            (*s) = nullptr;           
            return temp;
        }
        else {
            cout << "Could not exit student : there are no students to be exited\n";
            return nullptr;
        }
    }
    virtual ~STAIRS() {
            *s = nullptr;
            delete *s;
        
        delete[]s;
    }
};

class CORRIDOR: public LOCATION{
    const location corridor = 3;
    STUDENT** s;

public:
    CORRIDOR() :LOCATION(corridor) {
            s = new STUDENT * ;
            *s = new STUDENT;
            *s = nullptr;
        
        cout << "A corridor has been created\n";
    }

    

    int getNo() {
        return current_number;
    }

    int enter(STUDENT* st) {       
        st->assignLocation(corridor);
        *s = st;
        current_number++;
        st->printT();
        cout << "enters corridor\n";            
        return 0;        
    }

    void printStudents() {
        for (int i = 0; i < current_number; i++) {
            s[i]->printWithFtg();
            cout << "\n";
        }
    }

    STUDENT* exit() {
        if (*s != nullptr) { //if there are students to be exited
            STUDENT* temp = *s; // temp points to the student 
            current_number--; // he is being exited so reduce the number of students
            (*s)->printT(); // print his name
            cout << "exits corridor\n";            
            *s = nullptr; // nullify the pointer 
            return temp; //return the pointer to the student
        }
        else {
            cout << "Could not exit student : there are no students to be exited\n";
            return nullptr; //returns nullptr
        }
    }

    virtual ~CORRIDOR() {
            *s = nullptr; 
            delete *s;
        
        delete[]s;
    }
};

class FLOOR : public LOCATION {
    CLASSROOM** c;
    CORRIDOR* cor;
    const location floor = 3;
    STUDENT** s;
    int junior_ftg;
    int senior_ftg;
public:
    FLOOR(int max_class, int tp , int jftg , int sftg, STUDENT **st):LOCATION(floor), junior_ftg(jftg), senior_ftg(sftg) {
        c = new CLASSROOM * [6];
        for (int i = 0; i < 3; i++) {
            c[i] = new JUNIOR_CLASS(max_class,tp, junior_ftg,st);
        }
        for (int i = 3; i < 6; i++) {
            c[i] = new SENIOR_CLASS(max_class,tp, senior_ftg,st);
        }

        cor = new CORRIDOR();
        current_number = 0;
        s = new STUDENT *;
        
            *s = new STUDENT;
            *s = nullptr;

    }

    CLASSROOM** getClass() { //returns classroom array
        return c;
    }

    CLASSROOM* getClass(int i) { // returns the i-th classroom
        return c[i];
    }

    CORRIDOR* getCorridor() {   
        return cor;
    }

    void print() {
        for (int i = 0; i < 6; i++) {
            c[i]->printID();
            cout << endl;
        }
    }

    void printClassroom(int i) {
        getClass(i)->getTeacher()->print();
    }

    void printStudents() {
        getCorridor()->printStudents();
    }    

    int enter(STUDENT* st) {
        cor->enter(st);
        st->printT();
        cout << "enters floor\n";
        st->printT();
        *s = st;
        current_number++;
        cout << "enters corridor\n";
        return 0;       
    }   

    virtual ~FLOOR() {
        for (int i = 0; i < number_per_floor; i++) {
            cout << "delete c " << i << endl;
            delete c[i];
        }
        delete[]c;
        delete cor;
        *s = nullptr;
        delete *s;        
        delete[]s;
    }
};

class randomGen {
    int* randd;
    int span;
    int i;
public:

    int getI() {
        return i;
    }

    randomGen(int min, int span) {

        randd = new int[span];
        this->span = span;
        this->i = 0;
        randNum(min, span);
    }

    int randNum(int min, int span) { // creates random numbers and stores them in an array from min to span-1

        int random = 0;
        if (span == 1) {
            randd[i] = min; //if span =1 then only one number is possible , min and store it in the array
            i++;
            return min;
        }
        int split1, split2;

        random = rand() % span + min; // random number from min to span-1 
        if (random == min + span - 1) { // if the random is the max number
            split1 = random - min; // split 1 is max nummber -1
            split2 = span - split1; 
            randd[i] = random; //store the max number 
            i++;
            randNum(min, split1); //call randNum from min to span -2 (split1) 
        }
        else if (random == min) { //if random is the min
            split1 = random - min; 
            split2 = span - split1 - 1; //split 2 in this case is span -1
            randd[i] = random; //store the rand
            i++;
            randNum(random + 1, split2); // call from random(min)+1 to span +min -1
        }
        else {
            split1 = random - min; // split 1 is random -min
            split2 = span - split1 - 1; // split 2 is span - split1 - 1
            randd[i] = random; //store
            i++;

            randNum(min, split1); 
            randNum(random + 1, split2);
        }
        return 0;
    }

    void print() { //prints the array of random numbers
        for (int i = 0; i < span; i++) {
            cout << "PRINT " << randd[i] << endl;
        }
    }

    int getRand(int i) { //gets a the i-th number from the array
        return randd[i];
    }

    ~randomGen() {
        delete[]randd;
    }
};

void createStudents(FLOOR **f, STUDENT **s, int max_class, int senior_points , int junior_points) {
    char* a;
    a = new char[20];
    int k = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 6; j++) {
            for (int z = 0; z < max_class; z++) {
                snprintf(a, 20, "%d", k + 1); //from int to string
                string str(a);
                s[k]->namePerson(a); //names
                f[i]->getClass(j)->assignStudent(*s[k]); //assigns student to class
                cout << "A new student has been created : ";
                s[k++]->printC();
            }
        }            
    }
    
    delete[]a;
}

void createClassrooms(CLASSROOM** c, int floor, int  max_class) {
    for (int i = 0; i < 6; i++) {
        c[i]->assignClassId(floor + 1, i + 1);
        cout << "Classroom "; c[i]->printID(); cout << " has been created!" << endl;
    }
}

void createTeachers(FLOOR **f, int number_of_classrooms, int teach_points) {
    char* a;
    a = new char[10];
    int num = 1;
    for (int i = 0; i < number_of_floors; i++) {
        for (int j = 0; j < number_per_floor; j++) {
            snprintf(a, 10, "%d", num);
            num++;
            string str(a);
            f[i]->getClass(j)->nameTeacher(a);
            f[i]->getClass(j)->assignTeacher();
            cout << endl;
            cout << "A new teacher has been created : ";
            f[i]->getClass(j)->getTeacher()->printC();
        }
    }
    delete[]a;
}

void createFloor(FLOOR** f,  int max_class) {
    for (int i = 0; i < 3; i++) {
        createClassrooms(f[i]->getClass(), i, max_class);
        cout << "FLOOR " << i + 1 << " IS NOW COMPLETE" << endl;
    }
}

int placeRand() { // random 0 or 1
    int i;
    i = rand() % 2;
    return i;
}

classroom_id selectRandomTeacher() { // generates random class id 
    classroom_id id;
    srand(time(0));
    id.floor = (rand() % 3);
    id.number = (rand() % 6);
    return id;
}

class SCHOOL {
    FLOOR** f;
    STUDENT** s;
    STAIRS stairs;
    YARD yard;
    int number_of_students;
    int Cclass;
    int senior_points;
    int junior_points;
    int teach_points;
    int students_in;
    int teachers_in;
    randomGen gen;
public:
    SCHOOL(int nos, int max_class, int sp , int jp, int tp) :gen(0, nos), yard(), stairs(),senior_points(sp),junior_points(jp),teach_points(tp) {
        number_of_students = 18 * max_class;
        s = new STUDENT*[nos];        
        f = new FLOOR * [3];
        for (int i = 0; i < 3; i++) {
            f[i] = new FLOOR(max_class, teach_points, junior_points , senior_points,s);
            cout << "Floor " << i + 1 << "has been created!" << endl;
        }
        Cclass = max_class;
        
        students_in = 0;
        teachers_in = 0;
    }

    void printRand() {
        gen.print();
    }

    void createSchool() {
        createFloor(f,  Cclass);        
        createStudents(f, s, Cclass,senior_points, junior_points);
        createTeachers(f, number_of_classrooms, teach_points);
        cout << "A School has been created \n";
    }  

    int go_to_yard(){
        static int i = 0;
        if(i < number_of_students){
            yard.enter(s[gen.getRand(i++)]);
            return 0;
        }
        return -1;
    }

    int go_to_stairs(){
        STUDENT *st = yard.exit();
        if(st != nullptr){
            stairs.enter(st);
            return 0;
        }
        return -1;
    }

    int go_to_floor(int &fid, int &cid){
        STUDENT *st = stairs.exit();
        if(st != nullptr){
            f[st->getFloorID() - 1]->enter(st);
            cid = st->getNumberID();
            fid = st->getFloorID();
            return 0;
        }
        return -1;
    }

    int go_to_class(int fid, int cid){
        STUDENT *st= f[fid-1]->getCorridor()->exit();
        if(st!= nullptr){
            f[fid-1]->getClass(cid -1)->enter(st);
            return 0;
        }
        return -1;
    }


    int placeTeacher(classroom_id id) {
        if (placeRand() == 0 && !f[id.floor]->getClass(id.number)->getInside()) {
            f[id.floor]->getClass(id.number)->getTeacher()->place();
            cout << "\nTeacher has been placed in class "; f[id.floor]->getClass(id.number)->getID().print(); cout << endl;
            teachers_in++;
            return 1;
        }
        else {
            return -1;
        }
    }

    int placeTeacher() {
        classroom_id id;
        if (placeRand() == 0) { //if the random gen of 0 and 1 is 0 
            for (int i = 0; i < number_of_floors; i++) { //for all the teachers
                for (int j = 0; j < 6; j++) {
                    id.floor = i;
                    id.number = j;
                    placeTeacher(id); //place or not place randomly 
                }
            }
            return teachers_in; //how many teachers are inside
        }
        else {
            return -1;
        }
    }

    int getStudents_in() {
        return students_in; 
    }

    int getTeachers_in() {
        return teachers_in;
    }

    void printSchool() {
        cout << "\n\n\n";
        cout << "School life consists of: \n";
        cout << "People in the schoolyard are: \n";
        yard.printStudents();
        cout << "\n\n\n";
        cout << "People in the stairs are: \n";
        stairs.printStudents();
        for (int i = 0; i < number_of_floors; i++) {
            cout << "\nStudents in floor's " << i + 1 << " corridor are: \n";
            f[i]->printStudents();
        }
        for (int i = 0; i < number_of_floors; i++) {
            for (int j = 0; j < 6; j++) {
                cout << "Students in class "; f[i]->getClass(j)->getID().print(); cout << " : \n";
                f[i]->getClass(j)->printStudents();
            }
        }
    }


    void insert(){
        int cid = 0;
        int fid=0;
        int y =0 ;
        int s =0;
        int f = 0;
        int c = 0;
        int a = 1;
        while(a){
            y = go_to_yard();
            s = go_to_stairs();
            f = go_to_floor(fid,cid);
            c = go_to_class(fid,cid);

            a = (y==0) || (s==0) || (f==0) || (c==0);

            placeTeacher();
        }        
    }

    void operate(const int n){
        for(int i = 0; i <3 ; i++){
            for(int j = 0 ; j < 6; j++){                
                f[i]->getClass(j)->operate(n);
            }
        }
    }

    virtual ~SCHOOL() {
        cout << "A School to be destroyed!\n";

        for (int i = 0; i < 3; i++) {
            delete f[i];
        }
        delete[]f;
        for(int i = 0; i < number_of_students; i++){
            delete s[i];
        }
        delete[]s;
    }
};

int main(int argc, char* argv[]) {

    int max_cl = 0;
    const int* max_class = &max_cl;
    int junior_p = 0;
    const int* junior_points = &junior_p;
    int senior_p = 0;
    const int* senior_points = &senior_p;
    int teach_p=0;
    const int* teach_points = &teach_p;
    int oper_hours = 0;
    const int *operation_hours = &oper_hours;

    srand(time(0));
    if (argc < 6) {
        cout << "INVALID ARGUMENTS\n";
        return -1;
    }
    else {
        max_cl = atoi(argv[1]);
        junior_p = atoi(argv[2]);
        senior_p= atoi(argv[3]);
        teach_p= atoi(argv[4]);
        oper_hours = atoi(argv[5]);

        if (max_cl > 0 && junior_p >0&& senior_points>0 && teach_p>0 && oper_hours>0 ) {
            const int students = *max_class * number_of_classrooms;

            randomGen gen(0, students);

            SCHOOL school(students, *max_class, *senior_points , *junior_points, *teach_points);

            school.createSchool();
            school.insert();
            school.operate(*operation_hours);
            school.printSchool();
        }else{
            cout << "INVALID ARGUMENTS\n";
            return -1;
        }    
    }        
    return 0;
}
