#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <random>
#include <algorithm>

using namespace std;


// Tuile* shuffleTuile(){
//      // To obtain a time-based se
//     //std::shuffle(puzzle.begin(), puzzle.end(), g);
//     std::random_shuffle(std::begin(puzzle), std::end(puzzle));
//     return puzzle;

// }


class Tuile {
public:
    char top;
    char bottom;
    char right;
    char left;
    bool dejaUtiliser = false;
   
    void setValue(char left, char top, char right, char bottom) {
        this->top = top;
        this->bottom = bottom;
        this->left = left;
        this->right = right;
    }
};
class MacMah {
public:
    int N;
    int taille =N*N;
    vector<Tuile>puzzle;
    vector<Tuile>T;
    vector<thread> threads;
    mutex m;
    bool success;
    MacMah(int N, vector<Tuile> puzzle) {
        this->N = N;
        for (int i = 0; i <  N* N; ++i) {
            this->puzzle[i] = puzzle[i];
        }
    }



  

     bool vPosition(vector<Tuile> table, Tuile chess, int position) {
       
        if(position==0  &&  chess.top != chess.left){
            return false;
       }

       if(position!=0 &&  position < N && chess.top != table[position -1].top){
           
            return false;
           
       }

        if (position % N == 0 && position != 0 && chess.left != table[position - N].left) {
            return false;
        }

        if(position % N==N-1 && position >= N &&chess.right != table[position-N].right ){
            return false;
        }
        if (position == N*N -N && chess.left !=chess.bottom){
            return false;
        }

        if (position > N*N - N && position < N*N &&chess.bottom != table[position-1].bottom ){
            return false;
        }

        if (position - N >= 0) {
            if (chess.top != table[position - N].bottom) {
                return false;
            }
        }
        if (position % N != 0 && position != 0) {
            if (chess.left != table[position - 1].right) {
                return false;
            }
        }
        return true;
    }

      bool backtrackingSequentielle(vector<Tuile> table, int position) {
        if (position == N * N) {
            affichage(table);
            return true;
        }

        for (int i = 0; i < N * N; ++i) {
            if (!puzzle[i].dejaUtiliser) {
                if (vPosition(table, puzzle[i], position)) {
                    puzzle[i].dejaUtiliser = true;
                    table[position] = puzzle[i];
                    if (backtrackingSequentielle(table, position + 1)) {
                        return true;
                    }
                    puzzle[i].dejaUtiliser = false;
                }
            }
        }
        return false;
    }


vector<Tuile> shuffleTuile(){

    std::random_shuffle(std::begin(puzzle), std::end(puzzle));
    return puzzle;

}


void macmahonParallele(int position) {
   
    std::vector<std::thread> workerThreads(N);

    for (int t = 0; t < N; ++t) {
        workerThreads[t]=std::thread(&MacMah::backtrackingSequentielle,this,shuffleTuile(),position);
    }

    for (int t = 0; t < N; t++) {
        workerThreads[t].join();
    }

    
}

// void workerFunction(int position) {
//     for (int k = 0; k < N; ++k) {
//         if (!puzzle[k].dejaUtiliser && vPosition(puzzle[k], position)) {
//             puzzle[k].dejaUtiliser = true;
//             T[position] = puzzle[k];

//             if (backtrackingSequentielle( position+1)) {
//                 std::lock_guard<std::mutex> lock(m);
//                 success = true;
//             }

//             puzzle[k].dejaUtiliser = false;
//         }
//     }
// }

   void affichage(vector<Tuile> table) {
        for (int i = 0; i < N; ++i) {
            
            for (int k = 0; k < N; k++) {
                cout << "|  " << table[i * N + k].top << "  |";
            }
            cout << endl;
            for (int h = 0; h < N; h++) {
                cout << "|" << table[i * N + h].left << "   " << table[i * N + h].right << "|";
            }
            cout << endl;
            for (int l = 0; l < N; l++) {
                cout << "|  " << table[i * N + l].bottom << "  |";
            }
            cout << endl;
        }
    }


};

int main(int argc, char** argv) {
    ifstream file("mat6x6.txt");
    char left, top, right, bottom;
    int N;
    vector<Tuile> puzzle;
    Tuile c;

    if (file) {
        file >> N;
        file >> N;

        for (int i = 0; i < N * N; ++i) {
            file >> left;
            file >> top;
            file >> right;
            file >> bottom;

            c.setValue(left, top, right, bottom);

            puzzle.push_back(c);
        }

        file.close();
    }

    MacMah MacMah(N, puzzle);

   
    cout <<"Choisissez - P - pour paralléle OU -S- pour sequentielle \n";
    string choix ;
    cin>> choix;
    if(choix=="P"){
        MacMah.macmahonParallele(0);
    }else if(choix=="S"){
        MacMah.backtrackingSequentielle(MacMah.T,0);
    }

    return 0;
}

