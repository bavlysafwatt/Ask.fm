#include <bits/stdc++.h>

using namespace std;

vector<string> readlines(string path){
    vector<string> lines;
    fstream file_handler(path.c_str());

    if (file_handler.fail()) {
        cout << "\n\nERROR: Can't open the file\n\n";
        return lines;
    }
    string line;
    while(getline(file_handler, line)){
        if(line.size()==0)
            continue;
        lines.push_back(line);
    }
    file_handler.close();
    return lines;
}

void writeinfiles(string path, vector<string> lines, bool append = true){
    auto status = ios::in | ios::out | ios::app;

    if (!append)
        status = ios::in | ios::out | ios::trunc;	// overwrite

    fstream file_handler(path.c_str(), status);

    if (file_handler.fail()) {
        cout << "\n\nERROR: Can't open the file\n\n";
        return;
    }
    for(auto line : lines)
        file_handler << line << endl;
    file_handler.close();
    return;
}

int readchoice(int low, int high){
    cout << "\nEnter number in range " << low << " - " << high << ": ";
    int choice; cin >> choice;

    if (low <= choice && choice <= high)
        return choice;

    cout << "ERROR: invalid number...Try again\n";
    return readchoice(low, high);
}

int readmenu(vector<string> menu){
    cout << "Menu" << endl;
    for (int i = 0; i < menu.size() ; ++i) {
        cout << "\t" << i+1 << "- " << menu[i] << endl;
    }
    return readchoice(1, menu.size());
}

vector<string> SplitString(string s, string delimiter = ",") {
    vector<string> strs;

    int pos = 0;
    string substr;
    while ((pos = (int) s.find(delimiter)) != -1) {
        substr = s.substr(0, pos);
        strs.push_back(substr);
        s.erase(0, pos + delimiter.length());
    }
    strs.push_back(s);
    return strs;
}

int to_int(string n){
    istringstream iss(n);
    int num;
    iss >> num;
    return num;
}

struct user{
    int user_id;
    string user_name;
    string password;
    string name;
    string email;
    vector<int> question_from_me;
    map<int, vector<int>> question_thead;
    int allow_anonymous_questions;
    user(){
        user_id = allow_anonymous_questions = -1;
    }
    user(string line){
        vector<string> lines = SplitString(line);
        for(auto s : lines){
            user_id = to_int(lines[0]);
            user_name = lines[1];
            password = lines[2];
            name = lines[3];
            email = lines[4];
            allow_anonymous_questions = to_int(lines[5]);
        }
    }
    string to_str(){
        ostringstream oss;
        oss << user_id << "," << user_name << "," << password << "," << name << "," << email << "," << allow_anonymous_questions;
        return oss.str();
    }
};

struct question{
    int question_id;
    int parent_question_id;
    int from_user_id;
    int to_user_id;
    int is_anonymous_questions;
    string question_text;
    string answer_text;
    question(){
        question_id = parent_question_id = from_user_id = to_user_id = -1;
        is_anonymous_questions = 1;
    }

    question(string line){
        vector <string> lines = SplitString(line);
        question_id = to_int(lines[0]);
        parent_question_id = to_int(lines[1]);
        from_user_id = to_int(lines[2]);
        to_user_id = to_int(lines[3]);
        is_anonymous_questions = to_int(lines[4]);
        question_text = lines[5];
        answer_text = lines[6];
    }

    void print_to_question(){
        string pre;
        if(parent_question_id!=-1)
            pre = "\tThread: ";
        cout << pre << "Question Id: (" << question_id << ")";
        if(!is_anonymous_questions)
            cout << " from user id(" << from_user_id << ")";
        cout << "\t Question: " << question_text << "\n";

        if (answer_text != "")
            cout << pre << "\tAnswer: " << answer_text << "\n";
        cout << "\n";
    }

    void print_from_question(){
        cout << "Question Id: (" << question_id << ")";
        if(!is_anonymous_questions)
            cout << " !AQ ";
        cout << "To user id: (" << to_user_id << ")";
        cout << " Question: " << question_text;
        if(answer_text!="")
            cout << "\tAnswer: " << answer_text << endl;
        else
            cout << "\t Not Answered Yet" << endl;
    }

    void print_question(){
        if(parent_question_id!=-1){
            cout << "Thread Parent Question Id(" << parent_question_id << ") ";
        }
        cout << "Question Id (" << question_id << ") ";
        if(!is_anonymous_questions)
            cout << "from user(" << from_user_id << ") ";
        cout << "to user(" << to_user_id << ")";
        cout << "\tQuestion text: " << question_text << endl;
        cout << "\tAnswer text: " << answer_text << endl;
    }

    string to_str(){
        ostringstream oss;
        oss << question_id << "," << parent_question_id << "," << from_user_id << "," << to_user_id << "," << is_anonymous_questions << "," << question_text << "," << answer_text;
        return oss.str();
    }
};

struct UserManager{
    map<string, user> user_object;
    user current_user;
    int last_id;
    void Access_system() {
        int choice = readmenu({"Login", "Sign_Up"});
        if (choice == 1)
            Login();
        else
            Sign_up();
    }

    void Login(){
        load_database();
        while(true){
            cout << "Enter user name & password: ";
            cin >> current_user.user_name >> current_user.password;

            if (!user_object.count(current_user.user_name)) {
                cout << "\nInvalid user name or password. Try again\n\n";
                continue;
            }
            user user_exist = user_object[current_user.user_name];

            if (current_user.password != user_exist.password) {
                cout << "\nInvalid user name or password. Try again\n\n";
                continue;
            }
            current_user = user_exist;
            break;
        }
    }

    void Sign_up(){
        while(true){
            cout << "Enter username (no spaces): ";
            cin >> current_user.user_name;
            if (user_object.count(current_user.user_name))
                cout << "Already used. Try again\n";
            else
                break;
        }
        cout << "Enter a strong password: ";
        cin >> current_user.password;
        cout << "Enter your name: ";
        cin >> current_user.name;
        cout << "Enter you e-mail: ";
        cin >> current_user.email;
        cout << "Allow anonymous questions? (0 or 1): ";
        cin >> current_user.allow_anonymous_questions;

        current_user.user_id = ++last_id;
        user_object[current_user.user_name] = current_user;

        update_database(current_user);
    }

    void load_database(){
        user_object.clear();
        last_id = 0;
        vector<string> lines = readlines("D:\\Clion\\users.txt");
        for(auto line : lines){
            user User = user(line);
            user_object[User.user_name] = User;
            last_id = max(last_id, User.user_id);
        }
    }

    pair<int, int> read_user(){
        int id;
        cout << "Enter user id or -1 to cancel: ";
        cin >> id;
        if(id==-1) return make_pair(-1,-1);

        for(auto &pair : user_object){
            if(pair.second.user_id == id)
                return make_pair(id, pair.second.allow_anonymous_questions);
        }
        cout << "No such id, try again" << endl;
        read_user();
    }

    void list_users(){
        for(auto pair : user_object){
            user &User = pair.second;
            cout << "Id: " << User.user_id << "\tUsername: " << User.user_name << endl;
        }
    }

    void update_database(user User){
        string line = User.to_str();
        vector<string> lines(1, line);
        writeinfiles("D:\\Clion\\users.txt", lines);
    }
};
struct QuestionManager{
    map<int, question> question_object;
    map<int, vector<int>> question_thead;
    int last_id;
    void load_database(){
        last_id = 0;
        vector<string> lines = readlines("D:\\Clion\\questions.txt");
        for(auto line : lines){
            question Question = question(line);
            question_object[Question.question_id] = Question;
            if(Question.parent_question_id == -1)
                question_thead[Question.question_id].push_back(Question.question_id);
            else
                question_thead[Question.parent_question_id].push_back(Question.question_id);
            last_id = max(last_id, Question.question_id);
        }
    }

    void FillUserQuestions(user &User){
        User.question_from_me.clear();
        User.question_thead.clear();
        for(auto pair : question_object){
            question &q = question_object[pair.first];
            if(q.from_user_id == User.user_id)
                User.question_from_me.push_back(q.question_id);
            if(q.to_user_id == User.user_id){
                if(q.parent_question_id==-1)
                    User.question_thead[q.question_id].push_back(q.question_id);
                else
                    User.question_thead[q.parent_question_id].push_back(q.question_id);
            }
        }
    }
    void print_qusetion_to_me(user &current_user){
        if(current_user.question_thead.size()==0){
            cout << "No Questions" << endl;
            return;
        }
        for(auto pair : current_user.question_thead){
            for(auto id : pair.second){
                question qu = question_object[id];
                qu.print_to_question();
            }
        }
        cout << endl;
    }

    void print_qusetion_from_me(user &current_user){
        if(current_user.question_thead.size()==0){
            cout << "No Questions" << endl;
            return;
        }
        for(auto id: current_user.question_from_me){
            question qu = question_object[id];
            qu.print_from_question();
        }
        cout << endl;
    }

    void answer_question(user &User){
        int id;
        cout << "Enter Question Id or -1 to cancel: ";
        cin >> id;
        if(id==-1) return;

        if(!question_object.count(id)){
            cout << "No question with such id, Try again." << endl;
            answer_question(User);
            return;
        }
        question &q = question_object[id];
        if(q.to_user_id!=User.user_id){
            cout << "Invalid question, please try again." << endl;
            answer_question(User);
            return;
        }
        if(q.answer_text!=""){
            cout << "Question is already answred, answer will be updated." << endl;
        }
        cout << "Enter your answer: ";
        getline(cin, q.answer_text);
        getline(cin, q.answer_text);
        return;
    }

    void delete_question(user &User){
        int id1;
        cout << "Enter Question Id or -1 to cancel: ";
        cin >> id1;
        if(id1==-1) return;

        if(!question_object.count(id1)){
            cout << "No question with such id, Try again." << endl;
            delete_question(User);
        }
        question &q = question_object[id1];
        if(q.from_user_id!=User.user_id){
            cout << "Invalid question, please try again." << endl;
            delete_question(User);
        }
        vector<int> removed;
        if(question_thead.count(q.question_id)) {
            removed = question_thead[q.question_id];
            question_thead.erase(q.question_id);
        }
        else
            removed.push_back(q.question_id);

        for(auto pair : question_thead){
            vector<int> &v = pair.second;
            for (int i = 0; i < v.size(); ++i) {
                if(v[i]==id1){
                    v.erase(v.begin()+i);
                    break;
                }
            }
        }
        for(auto id : removed){
            question_object.erase(id);
        }
    }

    void ask_question(user &User, pair<int, int> to_user){
        question Question;
        if(!to_user.second){
            cout << "This user doesn't allow anonymous questions, your id will be shown." << endl;
            Question.is_anonymous_questions = 0;
        }
        else{
            cout << "Is anonymous questions?: (0 or 1): ";
            cin >> Question.is_anonymous_questions;
        }
        Question.to_user_id = to_user.first;
        Question.from_user_id = User.user_id;

        cout << "For thread question enter question id or -1 for new_question: ";

        int id; cin >> id;
        if(id!=-1){
            while (!question_thead.count(id)){
                cout << "No questions with such id, try another id: ";
                cin >> id;
            }
        }
        Question.parent_question_id = id;
        cout << "Enter question text: ";
        getline(cin, Question.question_text);
        getline(cin, Question.question_text);

        Question.question_id = ++last_id;
        question_object[Question.question_id] = Question;
        if(Question.parent_question_id==-1)
            question_thead[Question.question_id].push_back(Question.question_id);
        else
            question_thead[Question.parent_question_id].push_back(Question.question_id);
    }

    void feed(){
        for(auto pair : question_object){
            question Question = pair.second;
            if(Question.answer_text!="")
                Question.print_question();
        }
    }

    void update_database(){
        vector<string> lines;
        for(auto pair : question_object){
            lines.push_back(pair.second.to_str());
        }
        writeinfiles("D:\\Clion\\questions.txt", lines, false);
    }
};
struct askfmsystem{
    UserManager users_manager;
    QuestionManager questions_manager;

    void LoadDatabase(bool fill_user_questions = false) {
        users_manager.load_database();
        questions_manager.load_database();

        if (fill_user_questions)	// first time, waiting for login
            questions_manager.FillUserQuestions(users_manager.current_user);
    }

    void run() {
        LoadDatabase();
        users_manager.Access_system();
        questions_manager.FillUserQuestions(users_manager.current_user);

        vector<string> menu;
        menu.push_back("Print Questions To Me");
        menu.push_back("Print Questions From Me");
        menu.push_back("Answer Question");
        menu.push_back("Delete Question");
        menu.push_back("Ask Question");
        menu.push_back("List System Users");
        menu.push_back("Feed");
        menu.push_back("Logout");

        while (true) {
            int choice = readmenu(menu);
            LoadDatabase(true);

            if (choice == 1)
                questions_manager.print_qusetion_to_me(users_manager.current_user);
            else if (choice == 2)
                questions_manager.print_qusetion_from_me(users_manager.current_user);
            else if (choice == 3) {
                questions_manager.answer_question(users_manager.current_user);
                questions_manager.update_database();
            } else if (choice == 4) {
                questions_manager.delete_question(users_manager.current_user);
                questions_manager.FillUserQuestions(users_manager.current_user);
                questions_manager.update_database();
            } else if (choice == 5) {
                pair<int, int> to_user_pair = users_manager.read_user();
                if (to_user_pair.first != -1) {
                    questions_manager.ask_question(users_manager.current_user, to_user_pair);
                    questions_manager.update_database();
                }
            } else if (choice == 6)
                users_manager.list_users();
            else if (choice == 7)
                questions_manager.feed();
            else
                break;
        }
        run();
    }
};
int main() {
    askfmsystem service;
    service.run();
    return 0;
}
