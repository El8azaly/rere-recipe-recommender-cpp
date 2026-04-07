#include<bits/stdc++.h>
#include<fstream>
using namespace std;

string Breakfastfile = "data/Breakfast.txt";
string Lunchfile = "data/Lunch.txt";
string Dinnerfile = "data/Dinner.txt";
string Dessertfile = "data/Dessert.txt";
string Drinksfile = "data/Drinks.txt";
string Historyfile = "data/History.txt";
string Inventoryfile = "data/Inventory.txt";

string getCurrentDateTime() { //asked chatgpt for this one
    time_t now = time(nullptr);
    tm* t = localtime(&now);

    char buffer[100];
    strftime(buffer, sizeof(buffer), "%d-%m-%Y %I:%M:%S %p", t);

    string result(buffer);

    // Convert AM/PM to lowercase
    transform(result.begin(), result.end(), result.begin(),
              [](unsigned char c){ return tolower(c); });

    return result;
}

int valdchoice (int st, int end){
    cout<<"Press ("<<st<<" - "<<end<<"): "; int choice;
    while( !(cin >> choice) || choice < st || choice > end){
        cout<<"Invalid Choice. Please Enter a Choice From "<<st<<" to "<<end<<" : ";
        cin.clear();                 
        cin.ignore(10000, '\n'); 
    }
    return choice;
}

class Recipe{
    private:
    int ingcnt;
    string type;
    string name;
    string* ingredients;
    public:
    Recipe(int cnt, string n, string ty){
        ingcnt = cnt;
        name = n;
        type = ty;
    }
    ~Recipe(){ delete [] ingredients;}
    void AddIngredients(){
        ingredients = new string[ingcnt];
        cout<<"Add the recipe ingredients: "<<endl;
        cin.ignore();
        for(int i=0; i<ingcnt; i++){
            cout<<i+1<<". ";
            getline(cin,*(ingredients+i));
        }
    }
    string gettype(){return type;}
    int getcnt(){return ingcnt;}
    string getname(){return name;}
    string* getingredients(){return ingredients;}
};

int ShInv(){
    int cnt = 0;
    cout<<"---------------------------------------------------------------------\nThe current inventory is: "<<endl;
    ifstream input (Inventoryfile.c_str());
    if(input.fail()) cout<<"Couldn't Acces Inventory."<<endl;
    else{
        string x;
        while(getline(input,x)){
            cout<<x<<endl;
            cnt++;
        }
    }
    input.close();
        cout<<"---------------------------------------------------------------------"<<endl;

    return cnt;
}
void RecRec(){
    cout<<"---------------------------------------------------------------------"<<endl;
    string* wanted; int size;
    string* dishes; int dshcnt = 0;
    int* scores;  bool found = false;
    cout<<"How can RERE help you:\n1.What can my ingredients do?\n2.What can my inventory do?"<<endl;
    int choice = valdchoice(1,2);
    if(choice == 1){
        cout<<"Enter the number of ingredients you want to use: "; 
        while (!(cin >> size) || size <= 0){
            cout << "Invalid input! Please enter a positive integer: ";
            cin.clear();                 
            cin.ignore(10000, '\n');     
        }
        wanted = new string[size];
        cout<<"Enter the ingredients you want to use: "<<endl;
        cin.ignore();
        for(int i=0; i<size; i++){
            cout<<i+1<<". "; getline(cin,wanted[i]);
        }
    }
    else{
        size = ShInv();
        ifstream input (Inventoryfile.c_str());
        if(input.fail()) cout<<"Couldn't Acces Inventory."<<endl;
        else{
            wanted = new string[size];
            for(int i=0; i<size; i++)
            getline(input,wanted[i]);
        }
        input.close();
    }

    cout<<"Choose a meal: \n1.Breakfast\n2.Lunch\n3.Dinner\n4.Drink\n5.Dessert"<<endl;
    choice = valdchoice(1,5);
    string searchin;
    if(choice == 1) searchin = Breakfastfile;
    else if(choice == 2) searchin = Lunchfile;
    else if(choice == 3) searchin = Dinnerfile;
    else if(choice == 4) searchin = Drinksfile;
    else searchin = Dessertfile;
    ifstream input (searchin.c_str());
    if(input.fail()) cout<<"Couldn't Acces "<<searchin<<"."<<endl;
    else{
        string dish;
        while(getline(input,dish))
        dshcnt ++;
    }
    input.close();
    dishes = new string[dshcnt];
    scores = new int[dshcnt];
    
    input.open(searchin.c_str());
    if(input.fail()) cout<<"Couldn't Acces "<<searchin<<"."<<endl;
    else{
        string dish;
        int i = 0;
        while(getline(input,dish)){
            int stp = dish.find(':');
            dishes[i]=dish.substr(0,stp);
            dish = dish.substr(stp+2,dish.size()-stp-2);
            int ingcnt=1;
            for(int j=0; j<dish.size(); j++)
            if(dish[j]==',') ingcnt++;
            string compare [ingcnt];
            string save = "";
            int t = 0;
            for(int j=0; j<dish.size(); j++){
                if(dish[j]!=',') save += dish[j];
                else{
                    compare[t]=save;
                    save = "";
                    t++;
                    j++;
                }
            }
            compare[t]=save;
            /*cout<<"-----------------\n";
            cout<<"wanted: "; for(int l=0; l<size; l++) cout<<wanted[l]<<" ";
            cout<<"\ncompare: "; for(int k=0; k<ingcnt; k++) cout<<compare[k]<<" ";*/
            int score = 0;
            for(int k=0; k<ingcnt; k++){
                for(int l=0; l<size; l++){
                    if(compare[k]==wanted[l]){
                        score++;
                        break;
                    }
                }
            }
            //cout<<endl<<score<<endl;
            if(score==ingcnt){
                scores[i]=1;
                found = 1;
            }
            i++;
        }
    }
    input.close();
    cout<<"\n---------------------------------------------------------------------"<<endl;
    /*for(int h=0; h<dshcnt; h++){
        cout<<dishes[h]<<"  matching score = "<<scores[h]<<endl;
    }*/
    if(!found) cout<<"Sorry:( Your Recipe Book has no recipes to do with only these ingredients"<<endl;
    else{
        cout<<"You can cook: "<<endl;
        for(int l=0; l<dshcnt; l++){
            if(scores[l]==1){
                cout<<dishes[l]<<endl;
                ofstream output (Historyfile.c_str(),ios::app);
                output<<getCurrentDateTime()<<" ---> "<<dishes[l]<<" was recommended."<<endl;
                output.close();
            }
        }
    }
    delete [] wanted ;
    delete [] dishes ;
    delete [] scores ;
}

void addrec(){
    string name,type; int ingcnt;
    cout<<"Enter the recipe name: "; cin.ignore();
     getline(cin,name);
    cout<<"Enter the number of ingredients in the recipe: "; 
    while (!(cin>>ingcnt) || ingcnt <= 0){
            cout << "Invalid input! Please enter a positive integer: ";
            cin.clear();                 
            cin.ignore(10000, '\n');     
        }
    cout<<"You want to add it to:\n1.Breakfast\n2.Lunch\n3.Dinner\n4.Drinks\n5.Dessert"<<endl;
    int choice = valdchoice(1,5);
    if(choice == 1)         type = "Breakfast";
    else if (choice == 2)   type = "Lunch";
    else if (choice == 3)   type = "Dinner";
    else if (choice == 4)   type = "Drinks";
    else                    type = "Dessert";
    Recipe newrec(ingcnt, name, type);
    newrec.AddIngredients();
    if(newrec.gettype() == "Breakfast"){
        ofstream output (Breakfastfile.c_str(), ios::app);
        if(output.fail()) cout<<"Couldn't Access Breakfast."<<endl;
        else{
            output<<newrec.getname()<<": ";
            for(int i=0; i<newrec.getcnt(); i++){
                output<<*(newrec.getingredients()+i);
                if(i!=newrec.getcnt()-1) output<<", ";
            }
            output<<endl;
        }
        output.close();
    }
    else if(newrec.gettype() == "Lunch"){
         ofstream output (Lunchfile.c_str(), ios::app);
        if(output.fail()) cout<<"Couldn't Access Lunch."<<endl;
        else{
            output<<newrec.getname()<<": ";
            for(int i=0; i<newrec.getcnt(); i++){
                output<<*(newrec.getingredients()+i);
                if(i!=newrec.getcnt()-1) output<<", ";
            }
            output<<endl;
        }
        output.close();
    }
    else if(newrec.gettype() == "Dinner"){
         ofstream output (Dinnerfile.c_str(), ios::app);
        if(output.fail()) cout<<"Couldn't Access Dinner."<<endl;
        else{
            output<<newrec.getname()<<": ";
            for(int i=0; i<newrec.getcnt(); i++){
                output<<*(newrec.getingredients()+i);
                if(i!=newrec.getcnt()-1) output<<", ";
            }
            output<<endl;
        }
        output.close();
    }
    else if(newrec.gettype() == "Drinks"){
         ofstream output (Drinksfile.c_str(),ios::app);
        if(output.fail()) cout<<"Couldn't Access Drink."<<endl;
        else{
            output<<newrec.getname()<<": ";
            for(int i=0; i<newrec.getcnt(); i++){
                output<<*(newrec.getingredients()+i);
                if(i!=newrec.getcnt()-1) output<<", ";
            }
            output<<endl;
        }
        output.close();
    }
    else{ 
        ofstream output (Dessertfile.c_str(), ios::app);
        if(output.fail()) cout<<"Couldn't Access Dessert."<<endl;
        else{
            output<<newrec.getname()<<": ";
            for(int i=0; i<newrec.getcnt(); i++){
                output<<*(newrec.getingredients()+i);
                if(i!=newrec.getcnt()-1) output<<", ";
            }
            output<<endl;
        }
        output.close();
    }
    cout<<newrec.getname()<<" is added to "<<newrec.gettype()<<" succesfully!"<<endl;
}

void ShHistory(){
    cout<<"---------------------------------------------------------------------"<<endl;
    ifstream input (Historyfile.c_str());
    if(input.fail()) cout<<"Couldn't Acces History."<<endl;
    else{
        cout<<"Here is the recommendations history: "<<endl;
        string x;
        while(getline(input,x))
        cout<<x<<endl;
        cout<<"---------------------------------------------------------------------"<<endl;
    }
    input.close();
}

void AddInv(){
    cout<<"---------------------------------------------------------------------"<<endl;
    string x;
    cout<<"Enter the ingredient name: ";
    cin.ignore(); getline(cin,x);
    string content[1000]; bool found = 0; int ingcnt = 0;
    ifstream input (Inventoryfile.c_str());
    if(input.fail()) cout<<"Couldn't Acces Inventory.\n";
    else{
        int i=0;
        while(input>>content[i]){
            ingcnt++;
            if(content[i] == x) 
                found=1;
            i++;
        }
        if(!found){
            content[ingcnt]=x;
            ingcnt++;
        }
    }
    input.close();
    ofstream output (Inventoryfile.c_str());
    if(output.fail()) cout<<"Couldn't Acces Inventory."<<endl;
    else{
       for(int i=0; i<ingcnt; i++){
        output<<content[i]<<endl;
       }
    }
    output.close();
    cout<<x<<" is added succesfully to inventory!"<<endl;
    ShInv();
}

void ShBook(){
    cout<<"---------------------------------------------------------------------"<<endl;
    cout<<"Breakfast: "<<endl;
    ifstream input (Breakfastfile.c_str());
    if(input.fail()) cout<<"Couldn't Acces Breakfast."<<endl;
    else{
        string x;
        while(getline(input,x))
        cout<<x<<endl;
        cout<<"---------------------------------------------------------------------"<<endl;
    }
    input.close();

    cout<<"Lunch: "<<endl;
    input.open(Lunchfile.c_str());
    if(input.fail()) cout<<"Couldn't Acces Lunch."<<endl;
    else{
        string x;
        while(getline(input,x))
        cout<<x<<endl;
        cout<<"---------------------------------------------------------------------"<<endl;
    }
    input.close();
    
    cout<<"Dinner: "<<endl;
    input.open(Dinnerfile.c_str());
    if(input.fail()) cout<<"Couldn't Acces Dinner."<<endl;
    else{
        string x;
        while(getline(input,x))
        cout<<x<<endl;
        cout<<"---------------------------------------------------------------------"<<endl;
    }
    input.close();

    cout<<"Drinks: \n";
    input.open(Drinksfile.c_str());
    if(input.fail()) cout<<"Couldn't Acces Drinks."<<endl;
    else{
        string x;
        while(getline(input,x))
        cout<<x<<endl;
        cout<<"---------------------------------------------------------------------"<<endl;
    }
    input.close();

    cout<<"Desserts: \n";
    input.open(Dessertfile.c_str());
    if(input.fail()) cout<<"Couldn't Acces Dessert."<<endl;
    else{
        string x;
        while(getline(input,x))
        cout<<x<<endl;
        cout<<"---------------------------------------------------------------------"<<endl;
    }
    input.close();
}

int menu(){
    cout<<"---------------------------------------------------------------------"<<endl;
    cout<<"1.Recommend a Recipe.\n2.Add a New Recipe.\n3.Show Meals History.\n4.Add inventory.\n5.Show inventory\n6.Show All Recipes\n7.Exit Program"<<endl;
    int choice = valdchoice(1,7);
    if(choice == 1) {RecRec(); return 1;}
    else if(choice == 2) {addrec();    return 1;}
    else if(choice == 3) {ShHistory();return 1;}
    else if(choice == 4) {AddInv();return 1;}
    else if(choice == 5) {int x = ShInv();return 1;}
    else if(choice == 6) {ShBook();return 1;}
    else return 0;
}

int main(){
    cout<<endl<<"Welcome to \"RERE😉\" your lovely dining table friend to REcommend a REcipe!\n"<<"How can I help you?"<<endl;
    while(true){
        if(menu()==0){
        cout<<"---------------------------------------------------------------------"<<endl;
        cout<<"Thanks for using RERE!\nBon Appetit👋\n";
        cout<<"---------------------------------------------------------------------"<<endl;
           return 0;
        }
        cout<<endl;
    }
}