#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <random>
#include <algorithm>
#include<atomic>

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
    
    Tuile *puzzle;
    Tuile T[100];
    vector<thread> threads;
    mutex m;
    bool success;

    MacMah(int N, Tuile *puzzle) {
        this->N=N;
        this->puzzle=puzzle;
    }

     bool vPosition(Tuile table[100], Tuile chess, int position) {
       
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

    void affichage(Tuile table[100]) {
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

bool backtrackingSequentielle(Tuile *plateau, Tuile localT[100], int position) {
 

    if(reussi){
     cout << "Thread ID: " << std::this_thread::get_id() << ", Position: " << position << endl;
     return true;

    }
    if (position == N * N) {
        
        if(!reussi){
            m.lock();
            reussi=true;
            affichage(localT);
            m.unlock();

        }
        
        return true;
    }

    for (int i = 0; i < N * N; ++i) {
        if (!plateau[i].dejaUtiliser) {
            if (vPosition(localT, plateau[i], position)) {
                plateau[i].dejaUtiliser = true;
                localT[position] = plateau[i];
                
                if (backtrackingSequentielle(plateau, localT, position + 1)) {
                    return true;
                }
                plateau[i].dejaUtiliser = false;
            }
        }
    }
    return false;
}



Tuile* shuffleTuile() {
    Tuile* shuffled = new Tuile[N * N];
    std::copy(puzzle, puzzle + N * N, shuffled);

    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(shuffled, shuffled + N * N, g);

    return shuffled;
}



std::atomic<bool> reussi =false;

void macmahonParallele(int position) {
    //std::vector<std::thread> workerThreads(N);
   
    for (int t = 0; t < N*N; ++t) {
        Tuile localT[100];
        threads.push_back(std::thread(&MacMah::backtrackingSequentielle,this,shuffleTuile(),localT, position));
    }

    for (int t = 0; t < N*N; t++) {
        threads[t].join();
    }
}

};

int main(int argc, char **argv) {
    ifstream file("mat5x5.txt");
    char left, top, right, bottom;
    int N;
    Tuile *puzzle;  
    Tuile c;

    if (file) {
        file >> N;
        file >> N;

        puzzle = new Tuile[N * N];

        for (int i = 0; i < N * N; ++i) {
            file >> left;
            file >> top;
            file >> right;
            file >> bottom;

            c.setValue(left, top, right, bottom);

            puzzle[i] =c;
        }

        file.close();
    }

    MacMah MacMah(N, puzzle);

    cout << "Choisissez - P - pour paralléle OU -S- pour sequentielle \n";
    string choix;
    cin >> choix;
    if (choix == "P") {
        MacMah.macmahonParallele(0);
    } else if (choix == "S") {
        MacMah.backtrackingSequentielle(puzzle, MacMah.T, 0);
    }

    

    return 0;
}

