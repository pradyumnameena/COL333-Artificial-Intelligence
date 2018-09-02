/* 
 * File:   Track.h
 * Author: Kapil Thakkar
 *
 * Created on 9 August, 2015, 9:53 AM
 */

#ifndef TRACK_H
#define	TRACK_H


#include "Session.h"

/**
 * Track maintains an array of sessions.
 * 
 * @author Kapil Thakkar
 *
 */

class Track {
private:
    Session *sessions;
    int sessionsInTrack;
public:
    Track();
    
    /**
     * Constructor : Creates Track with given number of Sessions
     * 
     */
    Track(int sessionsInTrack);
    
    
    /**
     * Set the paper at the slot index to the specified paper id number.
     * 
     * @param sessionIndex is the session to modify.
     * @param paperIndex is the index of the paper.
     * @param paperId is the id number of the paper.
     */
    void setPaper(int sessionIndex, int paperIndex, int paperId);
    
    
    /**
     * Get the number of sessions in the track.
     * 
     * @return the number of sessions in the track.
     */
    int getNumberOfSessions();
    
    
    /**
     * Get a specified session.
     * 
     * @param index is the index of the session in question.
     * @return the session at the index.
     */
    Session getSession(int index);
    
    
    /**
     * Set the session at the specified index to the new session.
     * 
     * @param index is the index of the session.
     * @param session is the new session
     */
    void setSession(int index, Session session);
};

#endif	/* TRACK_H */

