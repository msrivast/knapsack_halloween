#include <bits/stdc++.h>

using namespace std;

int T = 100;
const int num = 16;
int weights[num], values[num];

vector<pair<float,int>> bags[3], bags_copy[3];

random_device rd; // obtain a random number from hardware
mt19937 gen(rd()); // seed the generator
uniform_int_distribution<> distr(0, 2); // define the range

int bagweights[3], bagweights_copy[3];
// int best;

void insert_sorted(vector<pair<float,int>> &v, pair<float,int> elem){
    v.insert( upper_bound( v.begin(), v.end(), elem ), elem );
}

struct stats{
    int total=0,bagweights[3] = {0,0,0};
} best;

stats fit_candies_to_bags(){
    // int sum = 0;
    stats current;
    vector<pair<float,int>> removed_candies;
    for(int i = 0; i < 3; i++){
        // sort candy
        sort(bags[i].begin(),bags[i].end());
        // cull
        while(bagweights[i]>2000){
            int candy_num = bags[i][0].second;
            bagweights[i] -= weights[candy_num];
            removed_candies.push_back(make_pair(bags[i][0].first,candy_num));
            bags[i].erase(bags[i].begin());
        }
        for (auto it = bags[i].begin(); it != bags[i].end(); it++)
            current.total += values[(*it).second];
        current.bagweights[i] = bagweights[i];
    }
    // refill greedily, starting from the bag with least amount of space
    sort(removed_candies.begin(),removed_candies.end());
    for (auto it = removed_candies.rbegin(); it != removed_candies.rend(); it++){
        map <int,int> m = {{bagweights[0],0},{bagweights[1],1},{bagweights[2],2}};
        for (auto b:m){
            int candy_weight = weights[(*it).second];
            if (candy_weight<=2000 - b.first){
                bags[b.second].push_back(*it); 
                bagweights[b.second] += candy_weight;
                current.total += values[(*it).second];
                current.bagweights[b.second] = bagweights[b.second];
                break;
            }
        } 
    }
    return current;
}

void refresh_global_variables(){
    for (int i = 0; i < 3; i++){
        bags[i] = bags_copy[i];
        bagweights[i] = bagweights_copy[i];
    }
}
     
int main(){
    
    ifstream file("candy.txt");
    int i = 0;
    while (file >> weights[i] >> values[i++]);

    for (int t = 0; t < T; t++){
        // Lucky draw: Randomly distribute candy among the three bags
        redo: for(i = 0; i < 3; i++){
            bags_copy[i].clear();
            bagweights_copy[i] = 0;
        }
        for(i = 0; i < num; i++){
                int chosen = distr(gen);
                // bags[chosen].push_back(make_pair(weights[i],i));
                bags[chosen].push_back(make_pair((float)values[i]/weights[i],i));
                bags_copy[chosen].push_back(make_pair((float)values[i]/weights[i],i));
                bagweights[chosen] += weights[i];
                bagweights_copy[chosen] += weights[i];
                }
                // for (int i=0; i < 3; i++) cout << "bag #" << i+1 << ": " << bagweights[i] << "\n";
        for(i = 0; i < 3; i++)
            if(bagweights[i]<2000){
                cout << "hmmm...redoing...\n";
                goto redo;
            }
        
        best = fit_candies_to_bags();
        // cout << "At the start: " << best.total << "\n";
        // for (int i=0; i < 3; i++) cout << "bag #" << i+1 << ": " << best.bagweights[i] << "\n";

        
        refresh_global_variables();

        // send candy to a bag
        for(int j = 0; j<3; j++){
            int i = 0;
            for (auto it = bags_copy[j].begin(); it != bags_copy[j].end(); it++){
                // copy and erase element pointed to by the iterator
                // insert_sorted(bags[(j+1)%3], make_pair((*it).first,(*it).second))
                int candy_num = (*it).second;
                
                for (int k = 1; k < 3;k++){
                    bags[(j+k)%3].push_back(make_pair((*it).first,candy_num));
                    bagweights[(j+k)%3] += weights[candy_num];
                
                    bags[j].erase(bags[j].begin() + i);
                    bagweights[j] -= weights[candy_num];

                    stats score = fit_candies_to_bags();
                    if  (score.total>best.total) best = score;

                    refresh_global_variables();

                    i++;
                }
            }
        }
        // cout << "At the end: " << best.total << "\n";
        cout << best.total << "\n";
        // for (int i=0; i < 3; i++) cout << "bag #" << i+1 << ": " << best.bagweights[i] << "\n";

    }

}

// plan -> randomly distribute all candy among three bags
// get an initial solution by removing excess candy. See if the
// lightest candy can find a home in any of the bags.

// Get neighboring solution by taking candy from bag 1 and 
// sending it to bag two. Remove excess candy from bag 2.
// Select greedily from the leftovers to see if something will fit
// bag 1.

// Another way to generate neighboring solutions might be to 
// start with the random config. Swap a candy between bag 1 and 2. Cull.
// See if the lightest ones fit any bag.
