//created by Maciej Miklas; mail: wajraia@wp.pl

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
using namespace std;
class data{
      private:
              unsigned char* list; //unsigned to make sort easier
              bool first_nibble; //for checking if writing 0-F to the first nibble or to the second one
              int count; //how much chars there are in the list
              int flip_index; //at which point chars in the list begin to have odd number of ones in byte
      public:
              data(char* filename); //automatically get chars from file
              ~data();
              void getCharactersFromFile(char* filename);
              void filterList(); //get rid off non 0-F chars
              int getCount();
              unsigned char* getList(); //actually no reason for that
              void printList(); //print list to the console
              bool checkParity(char pair); //check if there is even number of ones in byte
              void sortByOnes(); //get bytes with even number of ones to the left side of the list and odd ones to the right
              void printToFile(); //like printList() but to the file named "output.txt"
      };
int main(){
    data A("input.txt");
    A.filterList();
    A.sortByOnes();
    A.printToFile();
    return 0;
}
data::data(char* filename){
                   list=(unsigned char*)calloc(1,sizeof(char));
                   count=0;
                   first_nibble=true;
                   getCharactersFromFile(filename);
                   }
void data::getCharactersFromFile(char* filename){
                              fstream file;
                              file.open(filename,fstream::in);
                              if(!file.good()){
                                    cout<<"Access file error - no such file or no permission"<<endl;
                                    getchar();
                                    return;
                                    }
                              char temp;
                              int i=0;
                              while(!file.eof()){
                                      file.read(&temp,1);
                                      list[i]=(unsigned char)temp;
                                      i++;
                                      list=(unsigned char*)realloc(list,i*sizeof(char)+1); //make list one byte larger
                                      count++;
                                      }
                              file.close();
                              list[i]='\0'; //just in case
                              }
void data::filterList(){
                   unsigned char* temporary_list=(unsigned char*)calloc(count,sizeof(char));
                   int i=0,j=0,max=count;
                   count=0;
                   for(i;i<max;){
                       if(first_nibble){ //if first four bits are to set
                                if(list[i]>='0'&&list[i]<='9'){ //if it is from 0 to 9
                                                         temporary_list[j]=list[i]<<4; //shift to the left to have xxxx0000 in byte
                                                         first_nibble=!first_nibble;   //to make setting second nibble possible
                                                         }                             
                                else if(list[i]>='A'&&list[i]<='F'){ //if it is from A to F
                                     temporary_list[j]=(list[i]+9)<<4; //list[i]+9 because second nibble in 1 is 0001 and A is 0001 too 
                                     first_nibble=!first_nibble; //otherwise it would be impossible to distinguish A-F with 1-6
                                     }                           //so now A=1010
                                }
                       else{ //if last four bits are to set
                            if(list[i]>='0'&&list[i]<='9'){
                                                     temporary_list[j]=temporary_list[j]|(list[i]&0x0F); //list[i]&0x0F to get the
                                                     j++;                       //second nibble; bitwise OR to still have first nibble
                                                     first_nibble=!first_nibble;
                                                     count++; //bits in whole byte are set so get to the next one
                                                     }
                            else if(list[i]>='A'&&list[i]<='F'){
                                 temporary_list[j]=temporary_list[j]|((list[i]+9)&0x0F);
                                 j++;
                                 first_nibble=!first_nibble;
                                 count++;
                                 }
                            }
                       i++; //get to the next char in the list
                       }
                   temporary_list[j]='\0';
                   free(list); //could use realloc but wanted to clear memory
                   list=(unsigned char*)calloc(j+1,sizeof(char));
                   int k=0;
                   for(k;k<count;k++)list[k]=temporary_list[k];
                   list[k]='\0';
                   free(temporary_list);
                   }
void data::printList(){
     for(int i=0;i<count;i++){
            char temp[2];
            if( (list[i]&0xF0)>=0xA0 ){ 
                temp[0]=0x40|( ((list[i]&0xF0)>>4)-9 );                        //just "deciphering" stuff
                }
            else{
                 temp[0]=0x30|( (list[i]&0xF0)>>4 );
                 }
            if( (list[i]&0x0F)>=0x0A ){
                temp[1]=0x40|((list[i]&0x0F)-9);
                }
            else{
                 temp[1]=0x30|(list[i]&0x0F);
                 }
            for(int j=0;j<2;j++)cout<<temp[j];
            cout<<" "; //for better visibility
            }
     }
data::~data(){
              free(list);
              }
int data::getCount(){
    return count;
}
unsigned char* data::getList(){
      return list;
      }
void data::sortByOnes(){
     int value[count],k=count-1,j=0; //value[count] is an array in which there is stored info about to which index should move
     for(int i=0;i<count;i++){       //an element e.g. value[4]=12 means move 4th element to index 12
             if(checkParity(list[i])||list[i]==0){ value[j]=i;j++;} //if even # of ones then it should go to the left side of the array
             else{ value[k]=i;k--;} //else to the right side
             }
     if(j>k)flip_index=j;
     else flip_index=k;
     char temp;
     char temp_ch[count];
     for(int i=0;i<count;i++)temp_ch[i]=list[i]; //temporary list so we don't mess up order of elements
     for(int i=0;i<count;i++){
             list[i]=temp_ch[value[i]]; //rearranging elements
             }
     vector<unsigned char>temp_vec(list,list+count); //to make sort easier
     partial_sort(temp_vec.begin(),temp_vec.begin()+flip_index,temp_vec.begin()+flip_index);
     partial_sort(temp_vec.begin()+flip_index,temp_vec.end(),temp_vec.end());
     reverse(temp_vec.begin()+flip_index,temp_vec.end());
     for(int i=0;i<count;i++)list[i]=temp_vec[i]; //back to "normal" array
     }
bool data::checkParity(char pair){ //made in assembler because already had it in my notebook from studies; still some tweaks were needed
     __asm("\
          MOVB %AL, 8(%EBP) \n\
          MOVB $0x01, %AH \n\
          MOVB $0x01, %BL \n\
          ET0: \n\
          TEST %AL, %AH \n\
          JZ ET1 \n\
          XOR $0x01, %BL \n\
          ET1: \n\
          SHL $0x01, %AH \n\
          JNC ET0 \n\
          MOVL %EBX, %EAX \
          ");                    
     }
void data::printToFile(){
     fstream file;
     file.open("output.txt",fstream::out|fstream::trunc);
     if(!file.good()){
                                    cout<<"Access file error - no permission to create one"<<endl;
                                    getchar();
                                    return;
                                    }
     for(int i=0;i<count;i++){
            char temp[2];
            if( (list[i]&0xF0)>=0xA0 ){
                temp[0]=0x40|( ((list[i]&0xF0)>>4)-9 );
                }
            else{
                 temp[0]=0x30|( (list[i]&0xF0)>>4 );
                 }
            if( (list[i]&0x0F)>=0x0A ){
                temp[1]=0x40|((list[i]&0x0F)-9);
                }
            else{
                 temp[1]=0x30|(list[i]&0x0F);
                 }
            for(int j=0;j<2;j++)file<<temp[j];
            file<<" ";
            }
     file.close();
     }
