//created by Maciej Miklas; mail: wajraia@wp.pl

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
using namespace std;
int main(){
    int number_of_test_cases,n,k;
    cin>>number_of_test_cases;
    vector<int>number_of_elements;
    vector< vector<int> >elements(number_of_test_cases);
    vector<bool>does_exist;
    for(int i=0;i<number_of_test_cases;i++){
            cin>>n;
            number_of_elements.push_back(n);
            for(int j=0;j<number_of_elements[i];j++){
                    cin>>k;
                    elements[i].push_back(k);
                    }
            }
    for(int i=0;i<number_of_test_cases;i++){
            does_exist.push_back(false);
            for(int j=0;j<number_of_elements[i];j++){
                    int sum1,sum2;
                    if(elements[i].begin()+j==elements[i].begin())sum1=0;
                    else{
                         sum1=accumulate(elements[i].begin(),elements[i].begin()+j,0);
                         }
                    if(elements[i].begin()+j+1==elements[i].end())sum2=0;
                    else{
                         sum2=accumulate(elements[i].begin()+j+1,elements[i].end(),0);
                         }
                    cout<<sum1<<"\t"<<sum2<<endl;
                    if(sum1==sum2){
                                   does_exist.pop_back();
                                   does_exist.push_back(true);
                                   break;
                                   }
            }
            cout<<endl<<endl;
    }
    
    for(int i=0;i<number_of_test_cases;i++){
            if(does_exist[i])cout<<"Yes"<<endl;
            else cout<<"No"<<endl;
            }
    system("pause");
    return 0;
}
