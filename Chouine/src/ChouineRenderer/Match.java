/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ChouineRenderer;

/**
 *
 * @author VINCENTC
 */
public class Match
{
    public enum Gain
    {
        POINT, ROUND, MATCH;
    };
    
    private int m_WinPoints;
    private int m_ComputerPoints;
    private int m_UserPoints;
    private int m_WinRounds;
    private int m_ComputerRounds;
    private int m_UserRounds;
    private boolean m_ComputerWin;
    private boolean m_UserWin;
    
    public Match(int _points, int _rounds)
    {
        m_WinPoints = _points;
        m_WinRounds = _rounds;
        
        m_ComputerPoints = 0;
        m_ComputerRounds = 0;
        m_UserPoints = 0;
        m_UserRounds = 0;
        
        m_ComputerWin = false;
        m_UserWin = false;
    }
    
    public boolean isMatchOver()
    {
        return m_ComputerWin||m_UserWin;
    }
    
    public int getWinPoints()
    {
        return m_WinPoints;
    }
    
    public int getWinRonds()
    {
        return m_WinRounds;
    }
    
    public int getComputerPoints()
    {
        return m_ComputerPoints;
    }
    
    public int getUserPoints()
    {
        return m_UserPoints;
    }
    
    public int getComputerRounds()
    {
        return m_ComputerRounds;
    }
    
    public int getUserRounds()
    {
        return m_UserRounds;
    }
    
    public boolean isComputerWinner()
    {
        return m_ComputerWin;
    }
    
    public boolean isUserWinner()
    {
        return m_UserWin;
    }
    
    public Gain newComputerPoint()
    {
        Gain gain = Gain.POINT;
        m_ComputerPoints++;
        if (m_ComputerPoints >= m_WinPoints)
        {
            m_ComputerPoints = 0;
            m_UserPoints = 0;
            m_ComputerRounds++;
            gain = Gain.ROUND;
            if (m_ComputerRounds >= m_WinRounds)
            {
                m_ComputerWin = true;
                gain = Gain.MATCH;
            }
        }
        return gain;
    }
    
    public Gain newUserPoint()
    {
        Gain gain = Gain.POINT;
        m_UserPoints++;
        if (m_UserPoints >= m_WinPoints)
        {
            m_ComputerPoints = 0;
            m_UserPoints = 0;
            m_UserRounds++;
            gain = Gain.ROUND;
            if (m_UserRounds >= m_WinRounds)
            {
                m_UserWin = true;
                gain = Gain.MATCH;
            }
        }
        return gain;
    }
}
