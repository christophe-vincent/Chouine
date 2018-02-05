/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ChouineRenderer;

import com.jme3.asset.AssetManager;
import com.jme3.math.Quaternion;
import com.jme3.scene.Node;
import java.util.List;
import com.jme3.math.Vector3f;
import java.util.LinkedList;

/**
 *
 * @author vincentc
 */
public class CardManager
{

    static public final int COMPUTER_ID = 0;
    static public final int PLAYER_ID = 1;
    Node rootNode;
    AssetManager assetManager;
    private List<Card> m_PickCards;
    private LinkedList<Card> m_UserCards;
    private LinkedList<Card> m_UserWinCards;
    private List<Card> m_ComputerCards;
    private LinkedList<Card> m_ComputerWinCards;
    private CardFactory m_Card;
    private Card m_ComputerCard;
    private Card m_UserCard;

    CardManager(AssetManager _manager, Node _node)
    {
        rootNode = _node;
        assetManager = _manager;
        m_Card = new CardFactory(_manager, _node);
        m_ComputerCard = null;
        m_UserCard = null;
        m_UserCards = new LinkedList();
        m_ComputerCards = new LinkedList();
        m_ComputerWinCards = new LinkedList();
        m_UserWinCards = new LinkedList();
        m_PickCards = new LinkedList();
    }
    
    public void init()
    {
        if (m_ComputerCard != null)
        {
            m_ComputerCard.remove(rootNode);
            m_ComputerCard = null;
        }
        if (m_UserCard != null)
        {
            m_UserCard.remove(rootNode);
            m_UserCard = null;
        }
        for (Card c : m_UserCards)
        {
            c.remove(rootNode);
        }
        m_UserCards.clear();
        
        for (Card c : m_ComputerCards)
        {
            c.remove(rootNode);
        }
        m_ComputerCards.clear();
        
        for (Card c : m_ComputerWinCards)
        {
            c.remove(rootNode);
        }
        m_ComputerWinCards.clear();
        
        for (Card c : m_UserWinCards)
        {
            c.remove(rootNode);
        }
        m_UserWinCards.clear();
        
        for (Card c : m_PickCards)
        {
            c.remove(rootNode);
        }
        m_PickCards.clear();
    }

    private void RandomifyPos(Card _card, float _level)
    {
        // choose small values
        float x, y;

        // value between 0 and few mm
        x = (float) (_level * 10 * (Math.random()-0.5) * Positions.SCALE / 1000);
        y = (float) (10 * (Math.random()-0.5) * Positions.SCALE / 1000);

        _card.move(x, y, 0);

        // choose small angle
        x = (float) (_level * (Math.random()-0.5) / 10);

        _card.rotate(0, 0, x);
    }

    public boolean IsMoveable(String _name)
    {
        boolean ret = false;

        // only user can can move
        for (Card c : m_UserCards)
        {
            if (_name.equals(c.getName())
                || _name.equals(c.getName() + "b"))
            {
                // we found the card !
                ret = c.GetIsMoveable();
            }
        }
        return ret;
    }
    
    public boolean IsUserWinCard(String _name)
    {
        boolean ret = false;

        // only user can can move
        for (Card c : m_UserWinCards)
        {
            if (_name.equals(c.getName())
                || _name.equals(c.getName() + "b"))
            {
                // we found the card !
                ret = true;
            }
        }
        return ret;
    }
    
    public boolean IsComputerWinCard(String _name)
    {
        boolean ret = false;

        // only user can can move
        for (Card c : m_ComputerWinCards)
        {
            if (_name.equals(c.getName())
                || _name.equals(c.getName() + "b"))
            {
                // we found the card !
                ret = true;
            }
        }
        return ret;
    }
    
    void showComputerChoice(String _name)
    {
        for (Card c : m_UserCards)
        {
            if (_name.equals(c.getName())
                || _name.equals(c.getName() + "b"))
            {
                // we found the card !
                c.highlight();
            }
        }
    }

    public void SetPickCards(List<Cards.Card> _cards)
    {
        Card card;
        int index = _cards.size() - 1;
        int i;

        Vector3f pickPos = Positions.PICK_POS.clone();
        m_PickCards = new LinkedList();


        card = m_Card.createCard(
                _cards.get(_cards.size() - 1).displayName(),
                _cards.get(_cards.size() - 1).color(),
                _cards.get(_cards.size() - 1).value());
        card.setLocalTranslation(pickPos);
        card.rotate(0, 0, 1.57f);
        card.move(Positions.CARD_HEIGHT / 3, 0, 0);
        card.move(0, Positions.CARD_WIDTH / 4, 0);
        m_PickCards.add(card);
        pickPos.z += Positions.CARD_TICK;

        for (i = index; i > 0; i--)
        {
            card = m_Card.createCard(
                    "pick_" + String.valueOf(i), //_cards.get(i).displayName(),
                    Cards.Card.Color.BACK, //_cards.get(i).color(),
                    _cards.get(i).value());
            card.rotate(3.14f, 0, 0);
            card.setLocalTranslation(pickPos);
            RandomifyPos(card, 1);

            pickPos.z += Positions.CARD_TICK;
            card.SetState(Card.State.PICK);
            m_PickCards.add(card);
        }
    }

    public void RemovePickCard(int _count)
    {
        Card card;

        for (int i = 0; i < _count; i++)
        {
            if (m_PickCards.size() > 0)
            {
                card = m_PickCards.get(m_PickCards.size() - 1);
                card.remove(rootNode);
                m_PickCards.remove(card);
            }
        }
    }

    public void SetComputerCard(Cards.Card _card)
    {
        m_ComputerCard = m_Card.createCard(
                _card.displayName(),
                _card.color(),
                _card.value());
        m_ComputerCard.move(Positions.COMPUTER_PLAYED_POS);
    }

    public void SetUserCards(List<Cards.Card> _cards)
    {
        Card card;

        Vector3f loc = Positions.USER_CARD1_POS.clone();

        // remove old cards
        if (m_UserCards != null)
        {
            for (Card c : m_UserCards)
            {
                c.remove(rootNode);
            }
        }
        m_UserCards = new LinkedList();

        for (Cards.Card c : _cards)
        {
            card = m_Card.createCard(
                    c.displayName(),
                    c.color(),
                    c.value());
            card.SetIndex(_cards.indexOf(c));
            card.SetIsMoveable(true);
            card.setLocalTranslation(loc);

            loc = loc.add(Positions.CARD_SPACING, 0, 0);
            card.SetState(Card.State.USER);
            m_UserCards.add(card);
        }
    }

    public void SetComputerCards(List<Cards.Card> _cards)
    {
        Card card;

        Vector3f loc = Positions.COMPUTER_CARD1_POS.clone();
        
        // remove old cards
        if (m_ComputerCards != null)
        {
            for (Card c : m_ComputerCards)
            {
                c.remove(rootNode);
            }
        }
        m_ComputerCards = new LinkedList();

        for (Cards.Card c : _cards)
        {
            card = m_Card.createCard(
                    c.displayName(),
                    c.color(),
                    c.value());
            card.setLocalTranslation(loc);
            card.rotate(3.14f, 0, 0);
            if (Main.SHOW_COMPUTER_CARDS)
            {
                card.rotate(3.14f, 0, 0);
            }

            loc = loc.add(Positions.CARD_SPACING, 0, 0);
            card.SetState(Card.State.COMPUTER);
            m_ComputerCards.add(card);
        }
    }
    
    
    public void ShowComputerCards(boolean _show)
    {
            
        for (Card c : m_ComputerCards)
        {
            if (_show)
            {
                c.setLocalRotation(Vector3f.ZERO);
            }
            else
            {   
                c.rotate(3.14f, 0, 0);
            }
        }
    }
    
    public void SetComputerWinCards(List<Cards.Card> _cards)
    {
        Card card;
        float yOffset = 0;
        int index;
        Vector3f loc = Positions.COMPUTER_ALLCARDS_POS.clone();
        
        // remove old cards
        if (m_ComputerWinCards != null)
        {
            for (Card c : m_ComputerWinCards)
            {
                c.remove(rootNode);
            }
        }
        m_ComputerCards = new LinkedList();

        index = 0;
        for (Cards.Card c : _cards)
        {
            card = m_Card.createCard(
                    c.displayName(),
                    c.color(),
                    c.value());
            card.setLocalTranslation(loc);
            
            index++;
            if (index == 12)
            {
                loc.x = Positions.COMPUTER_ALLCARDS_POS.x;
                loc.y -= Positions.CARD_HEIGHT/2;
            }
            else
            {
                loc.x += (Positions.CARPET_WIDTH-Positions.CARD_WIDTH)/16;
            }
            loc.z += Positions.CARD_TICK;
            card.SetState(Card.State.COMPUTER_WIN);
            m_ComputerCards.add(card);
        }
    }
    
    public void SetUserWinCards(List<Cards.Card> _cards)
    {
        Card card;
        float yOffset = 0;
        int index;
        Vector3f loc = Positions.USER_ALLCARDS_POS.clone();
        
        // remove old cards
        if (m_UserWinCards != null)
        {
            for (Card c : m_UserWinCards)
            {
                c.remove(rootNode);
            }
        }
        m_UserCards = new LinkedList();
        
        if (_cards.size() > 12)
        {
            loc.y += Positions.CARD_HEIGHT/2;
        }

        index = 0;
        for (Cards.Card c : _cards)
        {
            card = m_Card.createCard(
                    c.displayName(),
                    c.color(),
                    c.value());
            card.setLocalTranslation(loc);
            
            index++;
            if (index == 12)
            {
                loc.x = Positions.USER_ALLCARDS_POS.x;
                loc.y -= Positions.CARD_HEIGHT/2;
            }
            else
            {
                loc.x += (Positions.CARPET_WIDTH-Positions.CARD_WIDTH)/16;
            }
            loc.z += Positions.CARD_TICK;
            card.SetState(Card.State.USER_WIN);
            m_UserCards.add(card);
        }
    }

    /*  public void UserPickCard()
     {
     PickCard(m_UserCards, Positions.USER_CARD1_POS, true);
     }

     public void ComputerPickCard()
     {
     PickCard(m_ComputerCards, Positions.COMPUTER_CARD1_POS, false);
     }
     */
    /* public void PickCard(List<Card> _cardList, Vector3f _position, Boolean _user)
     {
     Card c;
     Vector3f loc = _position;
     float[] offset = new float[5];
     offset[0] = 0;
     offset[1] = 1.1f * Positions.CARD_SPACING;
     offset[2] = 1.1f * Positions.CARD_SPACING;
     offset[3] = 1.1f * Positions.CARD_SPACING;
     offset[4] = 1.1f * Positions.CARD_SPACING;

     Boolean[] slots = new Boolean[5];
     for (int i = 0; i < 5; i++)
     {
     slots[i] = false;
     }

     c = m_PickCards.get(m_PickCards.size() - 1);
     m_PickCards.remove(c);

     // search empty slot
     for (Card uc : _cardList)
     {
     if (uc.GetIndex() < 5)
     {
     slots[uc.GetIndex()] = true;
     }
     }

     int i = 0;
     while ((slots[i] == true) & (i < 5))
     {
     i++;
     }

     if ((i < 5) && (slots[i] == false))
     {
     // we find the empty slot
     loc.x += offset[i]; //i*Positions.CARD_SPACING;
     //loc.x += i*Positions.CARD_SPACING;
     c.SetIndex(i);
     c.SetIsMoveable(_user);

     c.setLocalRotation(new Vector3f(0, 0, 0));
     c.setLocalTranslation(new Vector3f(0, 0, 0));
     if (!_user)
     {
     c.rotate(3.14f, 0, 0);
     }
     c.setLocalTranslation(new Vector3f(0, 0, 0.01f));

     c.move(loc);
     _cardList.add(c);
     }
     }  */
    
    private void searchUserCard(String _name)
    {
        m_UserCard = null;

        for (Card c : m_UserCards)
        {
            if (_name.equals(c.getName()))
            {
                m_UserCard = c;
            }
        }
    }

    public void SetUserChoice(String _name)
    {
        searchUserCard(_name);

        if (m_UserCard != null)
        {
            if (m_UserCards.contains(m_UserCard))
            {
                m_UserCards.remove(m_UserCard);
            }
        }
    }
    
    public void gotoOriginalPosition(String _name)
    {
        searchUserCard(_name);

        if (m_UserCard != null)
        {
            if (m_UserCards.contains(m_UserCard))
            {
                m_UserCard.setLocalTranslation(m_UserCard.getPosition());
                m_UserCard = null;
            }
        }
    }
    
    public boolean exchangeTrumpCard(int _playerIndex, String _7Trump)
    {
        Card userCard = null;
        List<Card> cardList;
        if (m_PickCards.isEmpty())
        {
            // no need to excahnge card, no cards in the pick...
            return true;
        }
        
        if (_playerIndex == COMPUTER_ID)
        {
            cardList = m_ComputerCards;
        }
        else if (_playerIndex == PLAYER_ID)
        {
            cardList = m_UserCards;
        }
        else
        {
            return false;
        }
        
        // check if user gets the trump 7
        for (Card c : cardList)
        {
            if (c.getName().equals(_7Trump))
            {
                userCard = c;
            }
        }
        
        if (userCard != null)
        {
            // card found, it can be exchanged
            Vector3f pos1, pos2;
            Card trump;
            
            pos1 = userCard.getPosition();
            pos2 = m_PickCards.get(0).getPosition();
            
            int index1 = cardList.indexOf(userCard);
            trump = m_PickCards.set(0, userCard);
            cardList.set(index1, trump);
            
            if (_playerIndex == COMPUTER_ID)
            {
                if (!Main.SHOW_COMPUTER_CARDS)
                {
                    userCard.rotate(3.14f, 0, 0);
                }
                trump.rotate(0, 3.14f, 0);
            }
            userCard.rotate(0, 0, 1.57f);
            userCard.setLocalTranslation(pos2);
            userCard.move(Positions.CARD_HEIGHT / 3, 0, 0);
            userCard.move(0, Positions.CARD_WIDTH / 4, 0);
            userCard.SetIsMoveable(false);
            userCard.SetState(Card.State.PICK);
            
            trump.rotate(0, 0, -1.57f);
            trump.setLocalTranslation(pos1);
            
            if (_playerIndex == PLAYER_ID)
            {
                trump.SetState(Card.State.USER);
                trump.SetIsMoveable(true);
            }
            else
            {
                trump.SetState(Card.State.COMPUTER);
            }
            
            return true;
        }
        
        return false;
    }

    public void announceCard(String _name)
    {
        searchUserCard(_name);

        if (m_UserCard != null)
        {
            m_UserCard.announcePos();
        }
    }

    public List<String> endAnnouce()
    {
        List<String> list = new LinkedList();
        for (Card c : m_UserCards)
        {
            if (c.isAnnouced())
            {
                c.announcePos();
                list.add(c.getName());
            }
        }
        return list;
    }

    public void RemovePlayedCards(int _winnerId)
    {
        Vector3f loc;
        List<Card> list;
        Card.State state;
        
        if (_winnerId == PLAYER_ID)
        {
            loc = new Vector3f(Positions.USER_WINCARDS_POS);
            list = m_UserWinCards;
            state = Card.State.USER;
        } else
        {
            loc = new Vector3f(Positions.COMPUTER_WINCARDS_POS);
            list = m_ComputerWinCards;
            state = Card.State.COMPUTER;
        }

        loc.z += list.size() * Positions.CARD_TICK;
        m_ComputerCard.setLocalTranslation(loc);
        m_ComputerCard.rotate(3.14f, 0, 0);
        m_ComputerCard.SetState(state);
        RandomifyPos(m_ComputerCard, 0.5f);
        list.add(m_ComputerCard);
        m_ComputerCard = null;

        loc.z += Positions.CARD_TICK;
        m_UserCard.setLocalTranslation(loc);
        m_UserCard.rotate(3.14f, 0, 0);
        m_UserCard.SetState(state);
        RandomifyPos(m_UserCard, 0.5f);
        list.add(m_UserCard);
        m_UserCard = null;

    }
    
    public void ShowUserWinCard(boolean _show)
    {

        if (_show)
        {
            Vector3f loc = Positions.USER_WINCARDS_POS.clone();
            loc.z += Positions.CARD_TICK*32;
            for (Card c : m_UserWinCards)
            {
                c.setLocalTranslation(loc);
                c.setLocalRotation(Vector3f.ZERO);
                loc.z += Positions.CARD_TICK;
            }
        }
        else
        {
            Vector3f loc = Positions.USER_WINCARDS_POS.clone();
        
            int index = 0;
            for (Card c : m_UserWinCards)
            {
                c.setLocalTranslation(loc);
                c.setLocalRotation(Vector3f.ZERO);
                c.rotate(3.14f, 0, 0);
                RandomifyPos(c, 1);
                index++;
                
                loc.z += Positions.CARD_TICK;
            }
        }
    }
    
    public void MoveUserWinCard(float _x, float _y)
    {
        boolean ret = false;
        float i = 0;

        // only user can can move
        for (Card c : m_UserWinCards)
        { 
            //c.setLocalTranslation(new Vector3f(_x, _y, 0));
            c.move(new Vector3f(i*_x, i*_y, 0));
            
            System.out.print(i+" ");
            i = i + (1/((float)m_UserWinCards.size()-1));
            
        }
        System.out.println("  "+1/((float)m_UserWinCards.size()-1));
        
    }
    
    public void ShowComputerWinCard(boolean _show)
    {

        if (_show)
        {
            // only user can can move
            if (m_ComputerWinCards.size() > 1)
            {
                for (int i=0; i<2; i++)
                {
                    m_ComputerWinCards.get(m_ComputerWinCards.size()-1-i).move(0, 0, Positions.CARD_TICK*32);
                    m_ComputerWinCards.get(m_ComputerWinCards.size()-1-i).rotate(3.14f, 0, 0);
                }
            }
        }
        else
        {
            // hide cards
            Vector3f loc = Positions.COMPUTER_WINCARDS_POS.clone();

            int index = 0;
            for (Card c : m_ComputerWinCards)
            {

                c.setLocalTranslation(loc);
                c.setLocalRotation(Vector3f.ZERO);
                c.rotate(3.14f, 0, 0);
                RandomifyPos(c, 1);
                index++;
                
                loc.z += Positions.CARD_TICK;
            }
        }
    }
    
    public void MoveComputerWinCard(float _x, float _y)
    {
        if (m_ComputerWinCards.size() > 1)
        {
            m_ComputerWinCards.get(m_ComputerWinCards.size()-1).move(new Vector3f(_x, _y, 0));
        } 
    }
}
