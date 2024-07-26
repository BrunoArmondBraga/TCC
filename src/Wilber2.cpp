#include <iostream>
#include <vector>

using namespace std;

const int INF = 1e9;  // Infinito positivo
const int NINF = -1e9;  // Infinito negativo

void print_vector(vector<int> s){
    for(int i = 0; i < s.size(); i++){
        cout << s[i] << " ";
    }
    cout << endl;
}

int find_Q(int last_index, int floor, int ceiling, vector<int> s){
    for(int i = last_index; i >= 1; i--){
        if(floor <= s[i] && s[i] <= ceiling){
            return i;
        }
    }
    return NINF;
}

int calculate_v(int s_i, int first_index, int last_index, bool is_max, vector<int> s){
    int result;

    if(is_max){
        result = NINF;
    }
    else{
        result = INF;
    }

    for(int j = first_index + 1; j <= last_index; j++){
        if(is_max){
            if(s[j] > result && s[j] < s_i){
                result = s[j];
            }
        }
        else{
            if(s[j] < result && s[j] > s_i){
                result = s[j];
            }
        }
    }

    return result;
}


int compute_kappa(vector<int> s, int i){
    if(i < 1){
        cout << "i < 1, inválido!" << endl;
        return 0;
    }
    if(i > s.size() - 1){
        cout << "i > s.size(), inválido" << endl;
        return 0;
    }
    if(i == 1){
        return 0;
    }

    vector<int> c;
    vector<int> w;
    vector<int> v;

    c.push_back(0); //ajuste de index
    w.push_back(0); //ajuste de index

    c.push_back(i-1);
    w.push_back(s[i-1]);

    

    if(w[1] < s[i]){
        v.push_back(INF);
    }
    else{
        v.push_back(NINF);
    }

    int l = 1;
    
    while(true){
        if(w[l] == s[i]){
            return l - 1;
        }
        else if(w[l] < s[i]){
            int max_Q = find_Q(c[l] - 1, s[i], v[l-1] - 1, s);
            if(max_Q == NINF){
                return l-1;
            }
            c.push_back(max_Q);
            w.push_back(s[c[l+1]]);
            v.push_back(calculate_v(s[i], c[l+1], c[l], true, s));
        }
        else{
            int max_Q = find_Q(c[l] - 1, v[l-1] + 1, s[i], s);
            if(max_Q == NINF){
                return l-1;
            }
            c.push_back(max_Q);
            w.push_back(s[c[l+1]]);
            v.push_back(calculate_v(s[i], c[l+1], c[l], false, s));
        }
        l++;
    }
}

int main(){
    int n, aux;
    vector<int> points;
    points.push_back(0); //ajuste de index
    cout << "Qual o número total de pontos? ";
    cin >> n;

    cout << "Digite todos as coordenadas X dos pontos em ordem crescente em Y." << endl;

    for(int i = 0; i < n; i++){
        cin >> aux;
        points.push_back(aux);
    }

    for(int i = 0; i < points.size(); i++){
        cout << points[i] << " ";
    }
    cout << endl;

    while(true){
        cout << "compute_kappa(i). i = ";
        cin >> aux;
        cout << endl << compute_kappa(points, aux) << endl;
    }

}