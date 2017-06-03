#include <iostream>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <conio.h>

enum {High_card=0, Pair, Two_pair, Three, Straight, Flush, Full, Quads, Straight_flush, Royal};
enum {CLUBS, DIAMONDS, HEARTS, SPADES};

using namespace std;

////////////////
//Functions

string rec(int val) //converting int value of card into name to be displayed
{
     if (val==1) return "A";
     else if (val==11) return "J";
     else if (val==12) return "Q";
     else if (val==13) return "K";
     else
     {
         stringstream ss;
         ss << val;
         return ss.str();
     }
}

int rerec(string value)  //converting string card value into integer
{
    if (value=="A") return 1;
    else if (value=="J") return 11;
     else if (value=="Q") return 12;
     else if (value=="K") return 13;
     else
     {
         stringstream ss(value);
         int val;
         ss>>val;
         return val;
     }
}

int recolor(char color)
{
    if (color==3) return HEARTS;
    else if (color==4) return DIAMONDS;
    else if (color==5) return CLUBS;
    else if (color==6) return SPADES;
}
///////////////////
//Classes

class Card
{
      public:
              char color;
              string value;
             void init(char c, string v) {color=c; value=v;}
             void show() {cout<<value<<color;}
};

class Player
{
      public:
      Card Hand[5];
      int cash, myBid, number;
      bool inGame;
      void showHand() {for(int i=0; i<5; i++) {Hand[i].show(); cout<<" ";} cout<<"\n";}
      int check();
      void change(vector<Card> v, int &lastFromDeck);
      void bid(int &mainbid, int &onTable, bool &checking);
};

void WhoWin(vector<Player> players, int prize);

//////////////////////
///Methods

int Player::check()
{
    vector<Card> v;
    Card memCard[2];
    int incolor=0;
    for(int i=0; i<5; i++) v.push_back(Hand[i]);
    
    for(int i=0; i<5; i++)//checking colors
    {
      if(Hand[0].color==Hand[i].color) incolor++;
    }
    
    vector<int> values;
    int qty[13];
    for(int i=0; i<13; i++) qty[i]=0;
    for(int i=0; i<13; i++)
    {
            int a=0;
            for(int j=0; j<5; j++) if(rerec(Hand[j].value)==i+1) a++;
            if(a>1) values.push_back(i+1);
            qty[i]=a;
    }
    for(int i=0; i<8; i++)
    {
            if(i==0)
            {
                    if(qty[i]==1&&qty[12]==1&&qty[11]==1&&qty[10]==1&&qty[9]==1)
                    {
                        if(incolor==5)
                        {
                                cout<<"Royal flush in "<<Hand[0].color<<endl;
                                return Royal;      
                        }
                        else
                        {
                            cout<<"Straight to "<<rec(i+4)<<endl;
                            return Straight;
                        }
                    }
            }
            else if(qty[i]==1&&qty[i+1]==1&&qty[i+2]==1&&qty[i+3]==1&&qty[i+4]==1)
            {
                if(incolor==5)
                        {
                                cout<<"Straight flush in "<<Hand[0].color<<endl;
                                return Straight_flush;      
                        }
                        else
                        {
                            cout<<"Straight to "<<rec(i+4)<<endl;
                            return Straight;
                        }
            }
    }
    if(incolor==5)
    {
          cout<<"Flush in "<<Hand[0].color<<endl;
          return Flush;
    }
    
    int k[2]={0,0};
    if(values.size()==1) k[0]=qty[values[0]-1];
    else if(values.size()==2) {k[0]=qty[values[0]-1]; k[1]=qty[values[1]-1];}
    
    if(k[0]==2&&k[1]==2)
    {
           cout<<"Two pair of "<<rec(values[0])<<" and "<<rec(values[1])<<endl;
           return Two_pair;
    }
    else if((k[0]==2||k[0]==3)&&(k[1]==2||k[1]==3))
    {
         cout<<"Full house of "<<rec(values[0])<<" and "<<rec(values[1])<<endl;
    }
    else switch(k[0])
    {
             case 2:
             { 
                  cout<<"One pair of "<<rec(values[0])<<endl;
                  return Pair;
             } break;
             case 3:
             { 
                  cout<<"Three of a kind "<<rec(values[0])<<endl;
                  return Three;
             } break;
             case 4: 
             { 
                  cout<<"Four of a kind "<<rec(values[0])<<endl;
                  return Quads;
             } break;
             default:
             {
                     cout<<"High card"<<endl;
                     return High_card;
             }
    }
}

void Player::change(vector<Card> v, int &lastFromDeck)
{
     int chosen[5]={0,0,0,0,0}; //0-not chosen, 1-chosen
     int pos=0; //position of cursor
     char c;
     while(true)
     {
          system("cls");
          cout<<"Choose cards to change (use arrows, SPACE to choose, ENTER to change):"<<endl;
          for(int i=0;i<5;i++)
          {
                  if(chosen[i]==1) cout<<(char)16;
                  if(pos==i) cout<<(char)254;
                  Hand[i].show();
                  if(chosen[i]==1) cout<<(char)17;
                  cout<<"   ";
          }
          c=getch();
          if(c==-32) //arrow key pressed
          {
                     c=getch();
                     if(c==77) {if(pos<4) pos++;} //RIGHT ARROW
                     else if(c==75) {if(pos>0) pos--;}//LEFT ARROW
          }
          else if(c==32) //SPACE pressed
          {
               if (chosen[pos]==0) chosen[pos]=1;
               else if(chosen[pos]==1) chosen [pos]=0;
          }
          else if(c==13) //ENTER pressed
          {
               cout<<"\nNew cards: ";
               for(int i=0; i<5; i++)
               {
                       if (chosen[i]==1)
                       {
                            Hand[i]=v[lastFromDeck];
                            Hand[i].show();
                            cout<<" ";
                            lastFromDeck++;
                       }
                       
               }
               getch();
               break;
          }
     }
}

void Player::bid(int &mainbid, int &onTable, bool &checking)
{
     int pos=0;
     while(true)
     {
            system("cls");
            cout<<"Your hand: ";
            showHand();
            cout<<"Actual poker hand: ";
            check();
            cout<<"Your cash: $"<<cash<<endl;
            cout<<"The main bid: $"<<mainbid<<"   Bank: $"<<onTable<<endl;
            if(pos==0) cout<<(char)254;
            cout<<"Bid"<<endl;
            if(pos==1)cout<<(char)254;
            cout<<"Check"<<endl;
            if(pos==2) cout<<(char)254;
            cout<<"Pass";
            char c;
            c=getch();
            if(c==-32) //arrow key pressed
            {
                     c=getch();
                     if(c==80) {if(pos<2) pos++;} //DOWN ARROW
                     else if(c==72) {if(pos>0) pos--;}//UP ARROW
            }
            else if(c==13) //ENTER pressed
            {
               bool move=false;
               switch(pos)
               {
                          case 0: //Bid
                          {
                               int bidcheck;
                               do
                               {
                                       cout<<"\nHow many would you like to bid?(type 0 if you'd like to do another move) $";
                                       cin>>bidcheck;
                                       if(bidcheck>cash) cout<<"You don't have enough cash to bid that sum."<<endl;
                                       if(bidcheck<mainbid) cout<<"The main bid is greater than what you want to bid."<<endl;
                               }while(bidcheck>cash);
                               cash-=bidcheck;
                               onTable+=bidcheck;
                               mainbid=myBid=bidcheck;
                               if(bidcheck!=0) move=true;
                          } break;
                          case 1: //Check
                          {
                               if(mainbid==0)
                               {
                                     cout<<"\nYou cannot check, if there is no bid.";
                                     getch();
                               }
                               else if(!(mainbid-myBid>cash))
                               {
                                     cash-=mainbid-myBid;
                                     myBid=mainbid;
                                     onTable+=mainbid-myBid;
                                     move=true;
                                     checking=true;
                               }
                               else
                               {
                                   cout<<"You don't have enough cash, so you pass";
                                   inGame=false;
                                   getch();
                                   move=true;
                                   checking=true;
                               }
                               
                          } break;
                          case 2: //Pass
                          {
                               inGame=false;
                               move=true;
                          }
               }
               if(move) break;
            }
     }
}

/////////////////
//MAIN function

int main(void)
{
    srand(time(NULL));
    vector<Card> v;
    Card temp;
    string s;
    int plays;
    cout<<"How many players? ";
    cin>>plays;
    Player players[plays];
    for(int i=0; i<plays; i++) {players[i].cash=500; players[i].number=i;}
    
    for (int i=3; i<=6; i++)
    {
        for (int j=1; j<=13; j++)
        {
            s=rec(j);
            temp.init(i,s);
            v.push_back(temp);
        }
    }
    
    while(true)
    {
       random_shuffle(v.begin(), v.end());
       for(int i=0; i<plays; i++) 
       {
               if(players[i].cash>0) players[i].inGame=true;
               players[i].myBid=0;
       }
    
       int k=0;
       for(int i=0; i<plays; i++) //1. turn
       {
            system("cls");
            for (int j=0; j<5; j++) //passing the cards to player
            {
                players[i].Hand[j]=v[k];
                k++;
            }
            cout<<"Player no."<<i+1<<" turn. Press ENTER to continue.";
            char c;
            c=getch();
            while(c!=13) getch();
            players[i].change(v,k); //changing cards
            system("cls");
            cout<<"Your hand: ";
            players[i].showHand();
            cout<<"Actual poker hand: ";
            players[i].check();
            getch();
       }
       int mainBid=0, onTable=0;
       bool checking[plays];
       int howManyChecks;
       for(int i=0; i<plays; i++) checking[i]=false;
       do//2. turn - the bidding
       {
           howManyChecks=0;
           for(int i=0; i<plays; i++) 
           {
               if(players[i].inGame)
               {
                     system("cls");
                     cout<<"Player no."<<i+1<<" turn. Press ENTER to continue.";
                     char c;
                     c=getch();
                     while(c!=13) c=getch();
                     players[i].bid(mainBid, onTable, checking[i]);
               }
               else checking[i]=true;
               if(checking[i]) howManyChecks+=1;
           }
       } while(howManyChecks<plays-1);
       
       vector<Player> vp;
       for(int i=0; i<plays; i++) vp.push_back(players[i]);
       WhoWin(vp, onTable);
       cout<<"\nEnd of play.";
       getch();
       cout<<endl;
    }
    return 0;
}

void WhoWin(vector<Player> players, int prize)
{
     int HighestCard=0, pokerhand[players.size()];
     vector<Player> v;
     system("cls");
     for(int i=0; i<players.size(); i++)
     {
             if(players[i].inGame)
             {
                  cout<<"Player no."<<i+1<<": ";
                  pokerhand[i]=players[i].check();
                  if(pokerhand[i]>HighestCard) HighestCard=pokerhand[i];
             }
     }
     
     for(int i=0; i<players.size(); i++) if(pokerhand[i]==HighestCard) v.push_back(players[i]);
     
     if(v.size()==1) for(int i=0; i<players.size(); i++) if(pokerhand[i]==HighestCard)
                                                  {
                                                        cout<<"Player no."<<i+1<<" wins!";
                                                        players[i].cash+=prize;
                                                        getch();
                                                  }
     else switch(HighestCard)
     {
          case High_card: case Straight:
          {
               cout<<HighestCard;
               int value[v.size()];
               for(int k=0; k<v.size(); k++)
               {
                   value[k]=0;
                   for(int i=0; i<5; i++)
                   {
                       int tempval;
                       tempval=rerec(v[k].Hand[i].value);
                       if (tempval==1) tempval=14; //in case of Ace
                       if (tempval>value[k]) value[k]=tempval;
                   }
               }
               int highval=0;
               for(int i=0; i<v.size(); i++)
               {
                   if(value[i]>highval) highval=value[i];
               }
               for(int i=0; i<v.size(); i++)
               {
                   if(value[i]==highval)
                   {
                       cout<<"Player no."<<v[i].number+1<<" wins!";
                       players[v[i].number].cash+=prize;
                       getch();
                   }
               }
          } break;
          case Pair:
          {
               int qty[v.size()];
               for (int j=0; j<v.size(); j++)
               {
                   qty[j]=0;
                   for(int i=0; i<13; i++)
                   {
                            int a=0;
                            for(int k=0; k<5; k++) if(rerec(v[j].Hand[k].value)==i+1) a++;
                            if(a==2) qty[j]=i;
                   }
               }
               int max=0;
               for (int j=0; j<v.size(); j++)
               {
                   if(qty[j]>max) max = qty[j];
               }
               for (int j=0; j<v.size(); j++)
               {
                   if(qty[j]<max) v.erase(v.begin()+j);
               }
               if (v.size()==1)
               {
                   cout<<"Player no."<<v[0].number+1<<" wins!";
                   players[v[0].number].cash+=prize;
                   getch();
               }
               else
               {
                   int value[v.size()];
                   for(int k=0; k<v.size(); k++)
                   {
                       value[k]=0;
                       for(int i=0; i<5; i++)
                       {
                           int tempval;
                           tempval=rerec(v[k].Hand[i].value);
                           if (tempval!=max+1)
                           {
                              if (tempval==1) tempval=14; //in case of Ace
                              if (tempval>value[k]) value[k]=tempval;
                           }
                       }
                   }
                   int highval=0;
                   for(int i=0; i<v.size(); i++)
                   {
                       if(value[i]>highval) highval=value[i];
                   }
                   for(int i=0; i<v.size(); i++)
                   {
                       if(value[i]==highval)
                       {
                           cout<<"Player no."<<v[i].number+1<<" wins!";
                           players[v[i].number].cash+=prize;
                           getch();
                       }
                   }
               }
          } break;
          case Two_pair:
          {
               int qty[v.size()][2];
               for (int j=0; j<v.size(); j++)
               {
                   qty[j][0]=qty[j][1]=0;
                   int h=0;
                   for(int i=0; i<13; i++)
                   {
                            int a=0;
                            for(int k=0; k<5; k++) if(rerec(v[j].Hand[k].value)==i+1) a++;
                            if(a==2) {qty[j][h]=i; h++;}
                   }
               }
               int max=0;
               for (int j=0; j<v.size(); j++)
               {
                   if(qty[j][0]>max) max = qty[j][0];
                   if(qty[j][1]>max) max = qty[j][1];
               }
               for (int j=0; j<v.size(); j++)
               {
                   if(qty[j][0]<max&&qty[j][1]<max) v.erase(v.begin()+j);
               }
               if (v.size()==1)
               {
                   cout<<"Player no."<<v[0].number+1<<" wins!";
                   players[v[0].number].cash+=prize;
                   getch();
               }
               else
               {
                   int value[v.size()];
                   for(int k=0; k<v.size(); k++)
                   {
                       value[k]=0;
                       for(int i=0; i<5; i++)
                       {
                           int tempval;
                           if(qty[k][0]!=max) tempval=qty[k][0];
                           
                           if (tempval!=max+1)
                           {
                              if (tempval==1) tempval=14; //in case of Ace
                              if (tempval>value[k]) value[k]=tempval;
                           }
                       }
                   }
                   int highval=0;
                   for(int i=0; i<v.size(); i++)
                   {
                       if(value[i]>highval) highval=value[i];
                   }
                   for(int i=0; i<v.size(); i++)
                   {
                       if(value[i]==highval)
                       {
                           cout<<"Player no."<<v[i].number+1<<" wins!";
                           players[v[i].number].cash+=prize;
                           getch();
                       }
                   }
               }
          } break;
          case Three: case Full:
          {
               int qty[v.size()];
               for (int j=0; j<v.size(); j++)
               {
                   qty[j]=0;
                   for(int i=0; i<13; i++)
                   {
                            int a=0;
                            for(int k=0; k<5; k++) if(rerec(v[j].Hand[k].value)==i+1) a++;
                            if(a==3) qty[j]=i;
                   }
               }
               int max=0;
               for (int j=0; j<v.size(); j++)
               {
                   if(qty[j]>max) max = qty[j];
               }
               for (int j=0; j<v.size(); j++)
               {
                   if(qty[j]==max)
                   {
                        cout<<"Player no."<<v[i].number+1<<" wins!";
                        players[v[i].number].cash+=prize;
                        getch();
                   }
               }
          } break;
          case Quads:
          {
               int qty[v.size()];
               for (int j=0; j<v.size(); j++)
               {
                   qty[j]=0;
                   for(int i=0; i<13; i++)
                   {
                            int a=0;
                            for(int k=0; k<5; k++) if(rerec(v[j].Hand[k].value)==i+1) a++;
                            if(a==4) qty[j]=i;
                   }
               }
               int max=0;
               for (int j=0; j<v.size(); j++)
               {
                   if(qty[j]>max) max = qty[j];
               }
               for (int j=0; j<v.size(); j++)
               {
                   if(qty[j]==max)
                   {
                        cout<<"Player no."<<v[i].number+1<<" wins!";
                        players[v[i].number].cash+=prize;
                        getch();
                   }
               }
          } break;
          case Flush: case Royal:
          {
               int color=0;
               for(int i=0; i<v.size(); i++)
               {
                       if(recolor(v[i].Hand[0].color)>color) color=recolor(v[i].Hand[0].color);
               }
               for(int i=0; i<v.size(); i++)
               {
                       if(recolor(v[i].Hand[0].color)==color)
                       {
                            cout<<"Player no."<<v[i].number+1<<" wins!";
                            players[v[i].number].cash+=prize;
                            getch();
                       }
               }
          } break;
          case Straight_flush:
          {
               int value[v.size()];
               for(int k=0; k<v.size(); k++) value[k]=0;
               for(int k=0; k<v.size(); k++)
               {
                   for(int i=0; i<5; i++)
                   {
                       int tempval;
                       tempval=rerec(v[k].Hand[i].value);
                       if (tempval==1) tempval=14; //in case of Ace
                       if (tempval>value[k]) value[k]=tempval;
                   }
               }
               int highval=0;
               for(int i=0; i<v.size(); i++)
               {
                   if(value[i]>highval) highval=value[i];
               }
               for(int i=0; i<v.size(); i++)
               {
                   if(value[i]<highval) v.erase(v.begin()+i);
               }
               if (v.size()==1)
               {
                   cout<<"Player no."<<v[0].number+1<<" wins!";
                   players[v[0].number].cash+=prize;
                   getch();
               }
               else //there are the same straight flushes, so we need to check colors
               {
                   int color=0;
                   for(int i=0; i<v.size(); i++)
                   {
                           if(recolor(v[i].Hand[0].color)>color) color=recolor(v[i].Hand[0].color);
                   }
                   for(int i=0; i<v.size(); i++)
                   {
                           if(recolor(v[i].Hand[0].color)==color)
                           {
                                cout<<"Player no."<<v[i].number+1<<" wins!";
                                players[v[i].number].cash+=prize;
                                getch();
                           }
                   }
               }
          } break;
     }
}
