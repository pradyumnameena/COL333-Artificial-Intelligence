/* 
 * File:   SessionOrganizer.cpp
 * Author: Kapil Thakkar
 * 
 */

#include "SessionOrganizer.h"
#include "Util.h"
#include "stdlib.h"
#include <cstdlib>
#include <random>
#include <cmath>

mt19937 rng;
int maximArr[10000];
SessionOrganizer::SessionOrganizer ( )
{
    parallelTracks = 0;
    papersInSession = 0;
    sessionsInTrack = 0;
    processingTimeInMinutes = 0;
    tradeoffCoefficient = 1.0;
}

SessionOrganizer::SessionOrganizer ( string filename )
{
    readInInputFile ( filename );
    conference = new Conference ( parallelTracks, sessionsInTrack, papersInSession );
}

double elapsedtime(struct timespec start){
    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC,&end);
    double s1,s2;
    s1 = (double)end.tv_sec + ((double)end.tv_nsec)/1000000000;
    s2 = (double)start.tv_sec + ((double)start.tv_nsec)/1000000000;
    return s1-s2;
}

void SessionOrganizer::organizePapers (struct timespec start)
{
    rng.seed(random_device()());
    uniform_int_distribution<mt19937::result_type> dist(0,98765);

    int paperCounter = 0;
    int i,j,k;
    for (i = 0;i<conference->getSessionsInTrack();i++){
        for (j = 0;j <conference->getParallelTracks();j++){
            for (k = 0;k <conference->getPapersInSession();k++){
                conference->setPaper ( j, i, k, paperCounter);
                paperCounter++;
            }
        }
    }
    this->copytoarr();


    // int lidx = 0;
    // double d = 0.0;
    // d = this->scoreOrganization();
    // double d2 = 0.0;
    // for(lidx = 0;lidx<100;lidx++){
    //     this->initial_state(distanceMatrix);
    //     d2 = this->scoreOrganization();
    //     // cout << d2 << " and d is " << d <<endl;
    //     if(d2>d){
    //         this->copytoarr();
    //         d = d2;
    //         this->random();
    //     }else{
    //          this->random();
    //     }
    // }

    double timeE = processingTimeInMinutes*60.0*0.99898;
    // if(elapsedtime(start)>=timeE){
    //         this->copytosch();
    //         cout << elapsedtime(start) << endl;
    //         break;
    // }
    
    rng.seed(random_device()());
    // double sc = this->scoreOrganization();
    // double d = this->flash_scorer(0,1,0,1,1,1,distanceMatrix,tradeoffCoefficient);
    // cout << sc << endl;
    // cout << d << endl; 
    // cout << sc+d << endl; 
    // conference->swap(0,0,1,1,1,1);
    // cout << this->scoreOrganization() << endl;
    // int counter = 0;
    while(true){
        // this->neighbour_gen(distanceMatrix,global_max,start);
        this->random(start);
        if(elapsedtime(start)>=0.1*timeE){
            this->copytosch();
//            cout << elapsedtime(start) << endl;
            break;
        }
        // counter++;
        
    }
    // cout<< counter<<endl;
    // cout << elapsedtime(start) << endl;
    // cout << "finished random state" << endl;

    // int lidx = 0;
    // double d = 0.0;
    // d = this->scoreOrganization();
    // double d2 = 0.0;
    // for(lidx = 0;lidx<100;lidx++){
    //     this->initial_state(distanceMatrix);
    //     d2 = this->scoreOrganization();
    //     // cout << d2 << " and d is " << d <<endl;
    //     if(d2>d){
    //         this->copytoarr();
    //         d = d2;
    //         this->random();
    //     }else{
    //          this->random();
    //     }
    // }
    // this->copytosch();

    // cout << "random output value " << conference->score(tradeoffCoefficient,distanceMatrix) << endl;

    this -> helper(start); 
}

void SessionOrganizer::copytosch(){
    int idx = 0;
    int numT = conference->getParallelTracks();
    int numS = conference->getSessionsInTrack();
    int numP = conference->getPapersInSession();
    int i,j,k;
    for(i = 0;i<numT;i++){
        for(j = 0;j<numS;j++){
            for(k = 0;k<numP;k++){
                conference->setPaper(i,j,k,maximArr[idx]);
                idx++;
            }
        }
    }
}

void SessionOrganizer::copytoarr(){
    int idx = 0;
    int numT = conference->getParallelTracks();
    int numS = conference->getSessionsInTrack();
    int numP = conference->getPapersInSession();
    int i,j,k;
    for(i = 0;i<numT;i++){
        for(j = 0;j<numS;j++){
            for(k = 0;k<numP;k++){
                maximArr[idx] = conference->getTrack(i).getSession(j).getPaper(k);
                idx++;
            }
        }
    }
}

double SessionOrganizer::flash_scorer(int tr1,int tr2,int se1,int se2,int p1,int p2,double ** distanceMatrix,int tradeoffCoefficient){
    double rv = 0.0;
    int numP = conference->getPapersInSession();
    int numS = conference->getSessionsInTrack();
    int numT = conference->getParallelTracks();
    Track t1 = conference->getTrack(tr1);
    Track t2 = conference->getTrack(tr2);
    Session s1 = conference->getTrack(tr1).getSession(se1);
    Session s2 = conference->getTrack(tr2).getSession(se2);
    int pap1 = s1.getPaper(p1);
    int pap2 = s2.getPaper(p2);
    int i,j,k;

    if(se1==se2){
        // computing change due to first
        for(i = 0;i<numP;i++){
            if(i!=p2){
                rv+=(1.0-distanceMatrix[pap1][s2.getPaper(i)]);
            }else{
                rv+=tradeoffCoefficient*(distanceMatrix[pap1][s2.getPaper(p2)]);
            }
        }

        for(i = 0;i<numP;i++){
            if(i!=p1){
                rv+=tradeoffCoefficient*(distanceMatrix[pap1][s1.getPaper(i)]);
            }
        }

        for(i = 0;i<numP;i++){
            if(i!=p1){
                rv-=(1.0-distanceMatrix[pap1][s1.getPaper(i)]);
            }
        }

        for(i = 0;i<numP;i++){
                rv-=tradeoffCoefficient*(distanceMatrix[pap1][s2.getPaper(i)]);
        }

        // computing change due to second
        for(i = 0;i<numP;i++){
            if(i!=p1){
                rv+=(1.0-distanceMatrix[pap2][s1.getPaper(i)]);
            }else{
                rv+=tradeoffCoefficient*(distanceMatrix[pap2][s1.getPaper(p1)]);
            }
        }

        for(i = 0;i<numP;i++){
            if(i!=p2){
                rv+=tradeoffCoefficient*(distanceMatrix[pap2][s2.getPaper(i)]);
            }
        }

        for(i = 0;i<numP;i++){
            if(i!=p2){
                rv-=(1.0-distanceMatrix[pap2][s2.getPaper(i)]);
            }
        }

        for(i = 0;i<numP;i++){
                rv-=tradeoffCoefficient*(distanceMatrix[pap2][s1.getPaper(i)]);
        }
    }else{
        // change in similarity
        for(i=0;i<numP;i++){
            if(i!=p1){
                rv+=(1.0-distanceMatrix[pap2][s1.getPaper(i)]);
            }   
        }

        for(i=0;i<numP;i++){
            if(i!=p2){
                rv+=(1.0-distanceMatrix[pap1][s2.getPaper(i)]);
            }   
        }

        for(i=0;i<numP;i++){
            if(i!=p1){
                rv-=(1.0-distanceMatrix[pap1][s1.getPaper(i)]);   
            }
        }

        for(i=0;i<numP;i++){
            if(i!=p2){
                rv-=(1.0-distanceMatrix[pap2][s2.getPaper(i)]);   
            }
        }

        // change in distance i.e. tradeoff parameter
        // Session s2;
        for(i = 0;i<numT;i++){
            if(i!=tr2){
                s2 = conference->getTrack(i).getSession(se2);
                for(j = 0;j<numP;j++){
                    rv+=(tradeoffCoefficient * distanceMatrix[pap1][s2.getPaper(j)]);
                }
            }
        }

        // Session s1;
        for(i = 0;i<numT;i++){
            if(i!=tr1){
                s1 = conference->getTrack(i).getSession(se1);
                for(j = 0;j<numP;j++){
                    rv+=(tradeoffCoefficient * distanceMatrix[pap2][s1.getPaper(j)]);
                }
            }
        }

        for(i = 0;i<numT;i++){
            if(i!=tr1){
                s1 = conference->getTrack(i).getSession(se1);
                for(j = 0;j<numP;j++){
                    rv-=(tradeoffCoefficient * distanceMatrix[pap1][s1.getPaper(j)]);
                }
            }
        }

        for(i = 0;i<numT;i++){
            if(i!=tr2){
                s2 = conference->getTrack(i).getSession(se2);
                for(j = 0;j<numP;j++){
                    rv-=(tradeoffCoefficient * distanceMatrix[pap2][s2.getPaper(j)]);
                }
            }
        }

    }
    // double d2 = this->scoreOrganization();
    // conference->swap(tr1,se1,p1,tr2,se2,p2);
    // double d1 = this->scoreOrganization();
    // rv = d1-d2;
    // // cout << rv << endl;
    // // cout << d2 << endl;
    // // cout << rv+d2 << endl;
    // // cout << d1 << endl;
    return rv;
}

void SessionOrganizer::helper (struct timespec start){
    double global_max = conference->score(tradeoffCoefficient,distanceMatrix);
    double d = 0.0;
    // int counter = 0;
    int f = 0;

    double timeE = processingTimeInMinutes*60.0*0.8888999;
	while(true){
        d = this->neighbour_gen(distanceMatrix,global_max,start);
        
        if(elapsedtime(start)>=timeE){
            this->copytosch();
            break;
        }

        if(d>global_max){
            global_max = d;
            this->copytoarr();
        }else{
            this->copytosch();
        }
        
        // cout << counter << " " << d << endl;
        // counter++;
    }

 //   cout << elapsedtime(start) << endl;   
}

void SessionOrganizer::initial_state(double ** distanceMatrix){
    int numT = conference->getParallelTracks();
    int numS = conference->getSessionsInTrack();
    int numP = conference->getPapersInSession();
    int size = numT * numS;
    int mc = 0;
    int size2 = numP;
    int i,j,i1,i2;
    int minidx = 0;
    int arr[3*size];
    double val[numP];
    double trackmin[numT];
    double valmax;
    for(i = 0;i<conference->getParallelTracks();i++){
        for(j = 0;j<conference->getSessionsInTrack();j++){
            minidx = j;
            valmax = 0.0;
            for(i1 = 0;i1<size2;i1++){
                for(i2 = 0;i2<size2;i2++){
                    if(i1!=i2){
                      val[i1]+=(1.0-distanceMatrix[conference->getTrack(i).getSession(j).getPaper(i1)][conference->getTrack(i).getSession(j).getPaper(i2)]);  
                    }
                }

                if(val[i1]>=valmax){
                    valmax = val[i1];
                    minidx = i1;
                }
            }
            arr[mc] = i;
            arr[mc+1] = j;
            arr[mc+2] = minidx;
            mc+=3;
        }
    }

    int fp = 0;
    int sp = 0;
    double diff = 0.0;
    for(i1=0;i1<size;i1++){
        for(i2=0;i2<size;i2++){
               diff = this->flash_scorer(arr[3*i1],arr[3*i2],arr[(3*i1)+1],arr[(3*i2)+1],arr[(3*i1)+2],arr[(3*i2)+2],distanceMatrix,tradeoffCoefficient);
                if(diff>0.0){
                    fp = i1;
                    sp = i2;
                    conference->swap(arr[3*fp],arr[(3*fp)+1],arr[(3*fp)+2],arr[3*sp],arr[(3*sp)+1],arr[(3*sp)+2]);
                    this->copytoarr();
                    break;
                } 
        }
        if(fp!=0){
            break;
        }
    }
}

double SessionOrganizer::neighbour_gen(double ** distanceMatrix,double maxtillnow,struct timespec start){
    
    int numT = conference->getParallelTracks();
    int numS = conference->getSessionsInTrack();
    int numP = conference->getPapersInSession();
    int size = numT * numS;
    int mc = 0;
    int size2 = numP;
    int arr[3*size];
    double val[numP];
    int i,j,i1,i2;
    int minidx;
    double valmax;
    
    for(i = 0;i<numT;i++){
        for(j = 0;j<numS;j++){
            minidx = j;
            valmax = 0.0;
            for(i1 = 0;i1<size2;i1++){
                for(i2 = 0;i2<size2;i2++){
                    val[i1]+=distanceMatrix[conference->getTrack(i).getSession(j).getPaper(i1)][conference->getTrack(i).getSession(j).getPaper(i2)];
                }
                if(val[i1]>=valmax){
                    valmax = val[i1];
                    minidx = i1;
                }
            }
            arr[mc] = i;
            arr[mc+1] = j;
            arr[mc+2] = minidx;
            mc+=3;
        }
    }

    int fp = 0;
    int sp = 0;
    double diff = 0.0;

    for(i1=0;i1<size;i1++){
        for(i2=0;i2<size;i2++){
            diff = this->flash_scorer(arr[3*i1],arr[3*i2],arr[(3*i1)+1],arr[(3*i2)+1],arr[(3*i1)+2],arr[(3*i2)+2],distanceMatrix,tradeoffCoefficient);
            if(diff>0.1){
                conference->swap(arr[3*i1],arr[3*i1+1],arr[3*i1+2],arr[3*i2],arr[3*i2+1],arr[3*i2+2]);
                fp = i1;
                sp = i2;
                this->copytoarr();
                conference->swap(arr[3*i1],arr[3*i1+1],arr[3*i1+2],arr[3*i2],arr[3*i2+1],arr[3*i2+2]);
                // cout<< this->scoreOrganization() <<endl;
                break;
            }

            double timeE = processingTimeInMinutes*60.0*0.8888999;
            if(elapsedtime(start)>=timeE){
                this->copytosch();
                break;
            }
        }
        if(fp!=0){
            break;
        }
        
        double timeE = processingTimeInMinutes*60.0*0.8888999;
        if(elapsedtime(start)>=timeE){
            this->copytosch();
            break;
        }
    }

    double eps = 0.001;
    if(diff<eps){

         double timeE = processingTimeInMinutes*60.0*0.8888999;
         double diff2 = diff;
         int i1,i2,j1,j2,k1,k2;
         int numT = conference->getParallelTracks();
         int numS = conference->getSessionsInTrack();
         int numP = conference->getPapersInSession();  
         
         rng.seed(random_device()());
         uniform_int_distribution<mt19937::result_type> dist(0,10000);
        
            while(diff2<=diff){
                     if(elapsedtime(start)>=timeE){
                         break;
                     }
                     
                     i1 = dist(rng)%(numT);
                     i2 = dist(rng)%(numT);
                     j1 = dist(rng)%(numP);
                     j2 = dist(rng)%(numP);
                     k1 = dist(rng)%(numS);
                     k2 = dist(rng)%(numS);
                     
                     diff2 = this->flash_scorer(i1,i2,k1,k2,j1,j2,distanceMatrix,tradeoffCoefficient);
                     if(diff2>diff){
                        conference -> swap(i1,k1,j1,i2,k2,j2);
                        this->copytoarr();
                        break; 
                     }
            }
                
                if(elapsedtime(start)>=timeE){
                    this->copytosch();
                }else{
                   this->copytoarr(); 
                }     
    }else{
        this->copytosch();
    }
    // cout << this->scoreOrganization() << endl; 
    return conference->score(tradeoffCoefficient,distanceMatrix);
}

void SessionOrganizer::random(struct timespec start){
         int i1,i2,j1,j2,k1,k2;
         int numT = conference->getParallelTracks();
         int numS = conference->getSessionsInTrack();
         int numP = conference->getPapersInSession();

         int f1=0;
         int f2=0;
         int g1=0;
         int g2=0;
         int h1=0;
         int h2=0;

         uniform_int_distribution<mt19937::result_type> dist(0,numT*numP*numS);

         i1 = dist(rng)%(numT);
         i2 = dist(rng)%(numT);
         j1 = dist(rng)%(numP);
         j2 = dist(rng)%(numP);
         k1 = dist(rng)%(numS);
         k2 = dist(rng)%(numS);

         // int iter = 0;
         double diff = -0.1;
         double max = 0.0;
         double timeE = processingTimeInMinutes*60.0*0.99899;
         while(diff<max){
            if(elapsedtime(start)>=timeE){
                    this->copytosch();
//                    cout << elapsedtime(start) << endl;
                    break;
            }
            double d1 = this->scoreOrganization();
            diff = this->flash_scorer(i1,i2,k1,k2,j1,j2,distanceMatrix,tradeoffCoefficient);
            if(diff>max){
                max = diff;
                f1 = i1;
                f2 = i2;
                g1 = j1;
                g2 = j2;
                h1 = k1;
                h2 = k2;
            }else{
                 i1 = dist(rng)%(numT);
                 i2 = dist(rng)%(numT);
                 j1 = dist(rng)%(numP);
                 j2 = dist(rng)%(numP);
                 k1 = dist(rng)%(numS);
                 k2 = dist(rng)%(numS);
                }
         }

         if(f1==0 && f2==0 && g1==0 && g2==0 && h1==0 && h2==0){
            // this->copytosch();
         }else{
            conference->swap(f1,h1,g1,f2,h2,g2);
            this->copytoarr();
         }
         
}

void SessionOrganizer::readInInputFile ( string filename )
{
    vector<string> lines;
    string line;
    ifstream myfile ( filename.c_str () );
    if ( myfile.is_open ( ) )
    {
        while ( getline ( myfile, line ) )
        {
            //cout<<"Line read:"<<line<<endl;
            lines.push_back ( line );
        }
        myfile.close ( );
    }
    else
    {
        cout << "Unable to open input file";
        exit ( 0 );
    }

    if ( 6 > lines.size ( ) )
    {
        cout << "Not enough information given, check format of input file";
        exit ( 0 );
    }

    processingTimeInMinutes = atof ( lines[0].c_str () );
    papersInSession = atoi ( lines[1].c_str () );
    parallelTracks = atoi ( lines[2].c_str () );
    sessionsInTrack = atoi ( lines[3].c_str () );
    tradeoffCoefficient = atof ( lines[4].c_str () );

    int n = lines.size ( ) - 5;
    double ** tempDistanceMatrix = new double*[n];
    for ( int i = 0; i < n; ++i )
    {
        tempDistanceMatrix[i] = new double[n];
    }


    for ( int i = 0; i < n; i++ )
    {
        string tempLine = lines[ i + 5 ];
        string elements[n];
        splitString ( tempLine, " ", elements, n );

        for ( int j = 0; j < n; j++ )
        {
            tempDistanceMatrix[i][j] = atof ( elements[j].c_str () );
        }
    }
    distanceMatrix = tempDistanceMatrix;

    int numberOfPapers = n;
    int slots = parallelTracks * papersInSession*sessionsInTrack;
    if ( slots != numberOfPapers )
    {
        cout << "More papers than slots available! slots:" << slots << " num papers:" << numberOfPapers << endl;
        exit ( 0 );
    }
}

double** SessionOrganizer::getDistanceMatrix ( )
{
    return distanceMatrix;
}

void SessionOrganizer::printSessionOrganiser ( char * filename)
{
    conference->printConference ( filename);
}

double SessionOrganizer::scoreOrganization ( )
{
    // Sum of pairwise similarities per session.
    double score1 = 0.0;
    for ( int i = 0; i < conference->getParallelTracks ( ); i++ )
    {
        Track tmpTrack = conference->getTrack ( i );
        for ( int j = 0; j < tmpTrack.getNumberOfSessions ( ); j++ )
        {
            Session tmpSession = tmpTrack.getSession ( j );
            for ( int k = 0; k < tmpSession.getNumberOfPapers ( ); k++ )
            {
                int index1 = tmpSession.getPaper ( k );
                for ( int l = k + 1; l < tmpSession.getNumberOfPapers ( ); l++ )
                {
                    int index2 = tmpSession.getPaper ( l );
                    score1 += 1 - distanceMatrix[index1][index2];
                }
            }
        }
    }

    // Sum of distances for competing papers.
    double score2 = 0.0;
    for ( int i = 0; i < conference->getParallelTracks ( ); i++ )
    {
        Track tmpTrack1 = conference->getTrack ( i );
        for ( int j = 0; j < tmpTrack1.getNumberOfSessions ( ); j++ )
        {
            Session tmpSession1 = tmpTrack1.getSession ( j );
            for ( int k = 0; k < tmpSession1.getNumberOfPapers ( ); k++ )
            {
                int index1 = tmpSession1.getPaper ( k );

                // Get competing papers.
                for ( int l = i + 1; l < conference->getParallelTracks ( ); l++ )
                {
                    Track tmpTrack2 = conference->getTrack ( l );
                    Session tmpSession2 = tmpTrack2.getSession ( j );
                    for ( int m = 0; m < tmpSession2.getNumberOfPapers ( ); m++ )
                    {
                        int index2 = tmpSession2.getPaper ( m );
                        score2 += distanceMatrix[index1][index2];
                    }
                }
            }
        }
    }
    double score = score1 + tradeoffCoefficient*score2;
    return score;
}
