#include "SL/VM/Runner/Runner.h"
#include <chrono>
#include <fstream>
#include <cstring>

using namespace std;

void run(const string&);

std::ostream *SL::Var::output = &cout;
std::istream *SL::Var::input = &cin;

int main(int argc,char* argv[]) {
    string str{
        R"(print ((2 + 3) * 2 * (1 + 2) * 2 - (1 - 1 * 1 + (1818 + 1 * 19 + (1))) + 4 * 2 + 1 - (1 + 1) * 1 - 1 / 1 + 81818181881 - (1 * 1 / 1) * (2 + 3) * 2 * (1 + 2) * 2 / 4 * 2 + 1 - (1 + 1) * 1 - 1 / 1 + 81818181881 - (1 * 1 / 1) * (2 + 3) * 2 * (1 + 2) * 2 / 4 * 2 + 1 - (1 + 1) * 1 - 1 / 1 + 81818181881 - (1 * 1 / 1) * (2 + 3) * 2 * (1 + 2) * 2 / 4 * 2 + 1 - (1 + 1) * 1 - 1 / 1 + 81818181881 - (1 * 1 / 1) * (2 + 3) * 2 * (1 + 2) * 2 / 4 * 2 + 1 - (1 + 1) * 1 - 1 / 1 + 81818181881 - (1 * 1 / 1) * (2 + 3) * 2 * (1 + 2) * 2 / 4 * 2 + 1 - (1 + 1) * 1 - 1 / 1 + 81818181881 - (1 * 1 / 1) * (2 + 3) * 2 * (1 + 2) * 2 - (1 - 1 * 1 + (1818 + 1 * 19 + (1))) + 4 * 2 + 1 - (1 + 1) * 1 - 1 / 1 + 81818181881 - (1 * 1 / 1) * (2 + 3) * 2 * (1 + 2) * 2 / 4 * 2 + 1 - (1 + 1) * 1 - 1 / 1 + 81818181881 - (1 * 1 / 1) * (2 + 3) * 2 * (1 + 2) * 2 / 4 * 2 + 1 - (1 + 1) * 1 - 1 / 1 + 81818181881 - (1 * 1 / 1) * (2 + 3) * 2 * (1 + 2) * 2 / 4 * 2 + 1 - (1 + 1) * 1 - 1 / 1 + 81818181881 - (1 * 1 / 1) * (2 + 3) * 2 * (1 + 2) * 2 / 4 * 2 + 1 - (1 + 1) * 1 - 1 / 1 + 81818181881 - (1 * 1 / 1) * (2 + 3) * 2 * (1 + 2) * 2 / 4 * 2 + 1 - (1 + 1) * 1 - 1 / 1 + 81818181881 - (1 * 1 / 1) * (2 + 3) * 2 * (1 + 2) * 2 - (1 - 1 * 1 + (1818 + 1 * 19 + (1))) + 4 * 2 + 1 - (1 + 1) * 1 - 1 / 1 + 81818181881 - (1 * 1 / 1) * (2 + 3) * 2 * (1 + 2) * 2 / 4 * 2 + 1 - (1 + 1) * 1 - 1 / 1 + 81818181881 - (1 * 1 / 1) * (2 + 3) * 2 * (1 + 2) * 2 / 4 * 2 + 1 - (1 + 1) * 1 - 1 / 1 + 81818181881 - (1 * 1 / 1) * (2 + 3) * 2 * (1 + 2) * 2 / 4 * 2 + 1 - (1 + 1) * 1 - 1 / 1 + 81818181881 - (1 * 1 / 1) * (2 + 3) * 2 * (1 + 2) * 2 / 4 * 2 + 1 - (1 + 1) * 1 - 1 / 1 + 81818181881 - (1 * 1 / 1) * (2 + 3) * 2 * (1 + 2) * 2 / 4 * 2 + 1 - (1 + 1) * 1 - 1 / 1 + 81818181881 - (1 * 1 / 1) * (2 + 3) * 2 * (1 + 2) * 2 - (1 - 1 * 1 + (1818 + 1 * 19 + (1))) + 4 * 2 + 1 - (1 + 1) * 1 - 1 / 1 + 81818181881 - (1 * 1 / 1) * (2 + 3) * 2 * (1 + 2) * 2 / 4 * 2 + 1 - (1 + 1) * 1 - 1 / 1 + 81818181881 - (1 * 1 / 1) * (2 + 3) * 2 * (1 + 2) * 2 / 4 * 2 + 1 - (1 + 1) * 1 - 1 / 1 + 81818181881 - (1 * 1 / 1) * (2 + 3) * 2 * (1 + 2) * 2 / 4 * 2 + 1 - (1 + 1) * 1 - 1 / 1 + 81818181881 - (1 * 1 / 1) * (2 + 3) * 2 * (1 + 2) * 2 / 4 * 2 + 1 - (1 + 1) * 1 - 1 / 1 + 81818181881 - (1 * 1 / 1) * (2 + 3) * 2 * (1 + 2) * 2 / 4 * 2 + 1 - (1 + 1) * 1 - 1 / 1 + 81818181881 - (1 * 1 / 1) * (2 + 3) * 2 * (1 + 2) * 2 - (1 - 1 * 1 + (1818 + 1 * 19 + (1))) + 4 * 2 + 1 - (1 + 1) * 1 - 1 / 1 + 81818181881 - (1 * 1 / 1) * (2 + 3) * 2 * (1 + 2) * 2 / 4 * 2 + 1 - (1 + 1) * 1 - 1 / 1 + 81818181881 - (1 * 1 / 1) * (2 + 3) * 2 * (1 + 2) * 2 / 4 * 2 + 1 - (1 + 1) * 1 - 1 / 1 + 81818181881 - (1 * 1 / 1) * (2 + 3) * 2 * (1 + 2) * 2 / 4 * 2 + 1 - (1 + 1) * 1 - 1 / 1 + 81818181881 - (1 * 1 / 1) * (2 + 3) * 2 * (1 + 2) * 2 / 4 * 2 + 1 - (1 + 1) * 1 - 1 / 1 + 81818181881 - (1 * 1 / 1) * (2 + 3) * 2 * (1 + 2) * 2 / 4 * 2 + 1 - (1 + 1) * 1 - 1 / 1 + 81818181881 - (1 * 1 / 1) * (2 + 3) * 2 * (1 + 2) * 2 - (1 - 1 * 1 + (1818 + 1 * 19 + (1))) + 4 * 2 + 1 - (1 + 1) * 1 - 1 / 1 + 81818181881 - (1 * 1 / 1) * (2 + 3) * 2 * (1 + 2) * 2 / 4 * 2 + 1 - (1 + 1) * 1 - 1 / 1 + 81818181881 - (1 * 1 / 1) * (2 + 3) * 2 * (1 + 2) * 2 / 4 * 2 + 1 - (1 + 1) * 1 - 1 / 1 + 81818181881 - (1 * 1 / 1) * (2 + 3) * 2 * (1 + 2) * 2 / 4 * 2 + 1 - (1 + 1) * 1 - 1 / 1 + 81818181881 - (1 * 1 / 1) * (2 + 3) * 2 * (1 + 2) * 2 / 4 * 2 + 1 - (1 + 1) * 1 - 1 / 1 + 81818181881 - (1 * 1 / 1) * (2 + 3) * 2 * (1 + 2) * 2 / 4 * 2 + 1 - (1 + 1) * 1 - 1 / 1 + 81818181881 - (1 * 1 / 1)))"
    };
    run(str);
    /*string codeString;
    if(argc>1){
        if(strcmp(argv[1],"-e") == 0){
            codeString = argv[2];
            cout << codeString << endl;
        }else{
            ifstream file{argv[1]};
            if(!file.is_open()){
                cout << "ERR: cannot open the file" << endl;
                return 1;
            }

            std::string line;
            while(getline(file,line)){
                codeString += line + '\n';
            }
        }
        run(codeString);
    }else{
        cout << "ERR: no input" << endl;
    }*/
    return 0;
}

void run(const string &codeString){
    try {
        double total = 0;
        auto start = chrono::high_resolution_clock::now();
        auto lexer = SL::Lexer{codeString};
        auto diff = chrono::high_resolution_clock::now() - start;
        total += chrono::duration<double, milli>(diff).count();
        *SL::Var::output << "Lexer:\t\t\t" << chrono::duration<double, milli>(diff).count() << "ms" << endl;
        start = chrono::high_resolution_clock::now();
        auto parser = SL::Parser{lexer};
        diff = chrono::high_resolution_clock::now() - start;
        total += chrono::duration<double, milli>(diff).count();
        *SL::Var::output << "Parser:\t\t\t" << chrono::duration<double, milli>(diff).count() << "ms" << endl;
        start = chrono::high_resolution_clock::now();
        auto code = SL::CodeGenerator{parser};
        diff = chrono::high_resolution_clock::now() - start;
        total += chrono::duration<double, milli>(diff).count();
        *SL::Var::output << "CodeGenerator:\t" << chrono::duration<double, milli>(diff).count() << "ms" << endl;
        *SL::Var::output << "------------Program Begin------------" << endl;
        start = chrono::high_resolution_clock::now();
        auto d = SL::Runner{code};
        diff = chrono::high_resolution_clock::now() - start;
        total += chrono::duration<double, milli>(diff).count();
        *SL::Var::output << "\n------------Program End------------" << endl;
        *SL::Var::output << "Runner:\t\t\t" << chrono::duration<double, milli>(diff).count() << "ms" << endl;
        *SL::Var::output << "Total:\t\t\t" << total << "ms" << endl;
    }catch (const std::exception &e){
        *SL::Var::output << e.what();
    }
}
