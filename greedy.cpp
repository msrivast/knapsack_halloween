#include<bits/stdc++.h>

using namespace std;

const int num = 16;
int weight[num], value[num];
pair<float,int> metrics[num];

vector<int> bags[3];
int bagweights[3];

int main(){
    // read file
    ifstream file("candy.txt");
    int i=0;
    while(file >> weight[i] >> value[i]){
        metrics[i].first = (float)value[i]/weight[i];
        metrics[i++].second = i;
    }

    sort(begin(metrics),end(metrics),greater<pair<float,int>>());
    // for (auto candy : metrics) cout << candy.second << " "; //<< candy.first << " " << candy.second <<" ";

    for (int i = 0; i < num; i++){
        bool taken = false;
        for (int j = 0; j < 3; j++){
            if (bagweights[j] + weight[metrics[i].second] <= 2000){
                bags[j].push_back(metrics[i].second);
                bagweights[j] += weight[metrics[i].second];
                taken = true;
                break;
            }
        }
        if (taken==false)
            cout << "candy #" << metrics[i].second << " with weight " << weight[metrics[i].second] << " and rank " << i << " is unused.\n";
    }
    
    int sum = 0;
    for (int i=0; i < 3; i++) cout << "bag #" << i+1 << ": " << bagweights[i] << "\n";

    for(auto bag : bags)
        for(int i : bag)
            sum += value[i];
    cout << "greedy: " << sum << "\n";
            
    

}