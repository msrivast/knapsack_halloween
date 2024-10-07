#include<bits/stdc++.h>

using namespace std;

int num = 16;
vector<int> weights, values;
int best; // 0 initialized

// vector<int> bags[3];
int bag_weights[3], best_bag_weights[3];

void read_file(string filename, vector<int> &weights, vector<int> &values){
    ifstream input_file(filename);
    int weight,value;
    while (input_file >> weight >> value ){
        weights.push_back(weight);
        values.push_back(value);
    }
}

int solve(int current_value, int i, int max_remaining_value){
    // base case
    if (i == num) return current_value;
    if (current_value+max_remaining_value<=best) return 0;

    
    int start=0;            // pruning
    if (i==0) start=2;
    if (i==1) start=1;

    for(int j=start;j<4;j++){
        int leaf_total = 0;
        if (j!=3 && weights[i] <= 2000 - bag_weights[j]){
            bag_weights[j] += weights[i];
            leaf_total = solve(current_value +  values[i], i+1, max_remaining_value-values[i]);
            if (leaf_total>best) {
                best = leaf_total;
                for(int k = 0; k<3; k++) best_bag_weights[k] = bag_weights[k];
            }
            bag_weights[j] -= weights[i];
        }
        if(j==3){
            leaf_total = solve(current_value, i+1,max_remaining_value-values[i]);
            if (leaf_total>best) {
                best = leaf_total;
                for(int k = 0; k<3; k++) best_bag_weights[k] = bag_weights[k];
            }
        }
    }
    return best;
}

int main(){

    read_file("candy.txt", weights, values);

    // for (int i = 0; i < weights.size(); i++)
    //     cout << weights[i] << " " << values[i] << "\n";
    int w_sum = 0, v_sum = 0;

    for (int i=0; i<num; i++){
        w_sum += weights[i];
        v_sum += values[i]; 
    }
    cout << w_sum << " " << v_sum << "\n";
    
    solve(0,0, v_sum);

    cout << best << "\n";
    for (int i=0; i<3; i++)
        cout << best_bag_weights[i] << " ";
    cout << "\n";
}