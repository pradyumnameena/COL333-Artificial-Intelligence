/* 
 * File:   Session.h
 * Author: Kapil Thakkar
 *
 */

#ifndef SESSION_H
#define	SESSION_H

#include <iostream>
#include <cstdlib>
using namespace std;

/**
 * Session holds an array of papers.
 * 
 * @author Kapil Thakkar
 *
 */

class Session {
    
private:    
    int *papers;            // array of paper    
    int papersInSession;    // number of papers
    
    
public:
    Session();
    
    
    /**
     * Constructor
     * 
     * @param papersInSession the number of papers in a session.
     */
    Session(int papersInSession);

    /**
     * Initialize the array of papers.
     * 
     * @param papersInSession the number of papers in a session.
     */
    void initPapers(int papersInSession);


    /**
     * Get the number of papers in a session.
     * 
     * @return the number of papers.
     */
    int getNumberOfPapers();

    /**
     * Get the id of the paper at the specified index
     * 
     * @param index the index of the paper
     * @return the id of the paper
     */
    int getPaper(int index);

    /**
     * Set the paper id at the specified index.
     * 
     * @param index is the index in the array
     * @param paperId is the id of the paper
     */
    void setPaper(int index, int paperId);

    /**
     * Print the papers present in current session 
     *
     */
    void printSession();
};

#endif	/* SESSION_H */

