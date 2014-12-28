/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Cards;

import java.util.LinkedList;

/**
 *
 * @author VINCENTC
 */
public class MoveNode
{

    private Card m_DonorCard;
    private Card m_OtherCard;
    private MoveNode m_Parent;
    private LinkedList<MoveNode> m_Childs;
    private int m_Score;     // score for player 0
    private int m_ScoreSum;  // score for each childs and sub child nodes
    private boolean m_isDonorWinner;
    private int m_DonorId;  // 0 or 1

    public MoveNode(MoveNode _parent,
            Card _donorCard,
            Card _otherCard,
            int _donorId,
            boolean _lastMove)
    {
        int der = 10;
        m_DonorCard = _donorCard;
        m_OtherCard = _otherCard;
        m_Parent = _parent;
        m_DonorId = _donorId;
        m_Score = 0;
        m_ScoreSum = 0;
        m_Childs = new LinkedList<>();
        m_isDonorWinner = true;
        
        if (m_DonorCard.isBetter(m_OtherCard))
        {
            m_isDonorWinner = false;
        }

        m_Score = m_DonorCard.getPoints() + m_OtherCard.getPoints();
        if (_lastMove)
        {
            m_Score += der;
        }
        
        if ((m_isDonorWinner && (m_DonorId != 0))
                || (!m_isDonorWinner && (m_DonorId == 0)))
        {
            m_Score = -m_Score;
        }
    }

    public Card getDonorCard()
    {
        return m_DonorCard;
    }

    public Card getOtherCard()
    {
        return m_OtherCard;
    }

    public MoveNode getParent()
    {
        return m_Parent;
    }

    public int getScore()
    {
        return m_Score;
    }
    
    public int getTotalScore()
    {
        return m_ScoreSum;
    }

    public void addScore(int _score)
    {
        m_ScoreSum += _score;
    }

    public boolean getIsWinner()
    {
        return m_isDonorWinner;
    }

    public int getDonorId()
    {
        return m_DonorId;
    }
}
