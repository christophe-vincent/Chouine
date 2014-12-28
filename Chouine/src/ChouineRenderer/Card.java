/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ChouineRenderer;

import com.jme3.material.Material;
import com.jme3.math.ColorRGBA;
import com.jme3.math.Quaternion;
import com.jme3.math.Vector3f;
import com.jme3.scene.Geometry;
import com.jme3.scene.Node;

/**
 *
 * @author vincentc
 */
public class Card
{
    public enum State
    {
        PICK,
        USER,
        COMPUTER,
        USER_WIN,
        COMPUTER_WIN
    }
    
    private Geometry m_CardFront;
    private Geometry m_CardBack;
    private boolean m_IsMoveable;
    private int m_Index; // only used for card that wait to be played
    private boolean m_isAnnouced;
    private Vector3f m_Position;
    private State m_State;

    Card(Geometry _cardf, Geometry _cardb)
    {
        m_CardFront = _cardf;
        m_CardBack = _cardb;
        m_IsMoveable = false;
        m_isAnnouced = false;
        m_Index = 0;
        m_Position = new Vector3f(0, 0, 0);
        m_State = State.PICK;
    }

    public void remove(Node _node)
    {
        if (m_CardFront != null)
        {
            _node.detachChild(m_CardFront);
            _node.detachChild(m_CardBack);
            m_CardBack = null;
            m_CardFront = null;
        }
    }

    public void SetIndex(int _index)
    {
        m_Index = _index;
    }

    public int GetIndex()
    {
        return m_Index;
    }
    
    public void SetState(State _state)
    {
        m_State = _state;
    }
    
    public State GetState()
    {
        return m_State;
    }

    public void SetIsMoveable(boolean _val)
    {
        m_IsMoveable = _val;
    }

    public boolean isAnnouced()
    {
        return m_isAnnouced;
    }

    public boolean GetIsMoveable()
    {
        return m_IsMoveable;
    }

    public String getName()
    {
        return m_CardFront.getName();
    }

    public Vector3f getPosition()
    {
        return m_Position;
    }
    
    public void highlight()
    {
        Material mat;
        mat = m_CardFront.getMaterial();
        mat.setColor("Color", new ColorRGBA(0.7f, 0.7f, 1, 1f));
        m_CardFront.setMaterial(mat);
    }

    public void rotate(float _x, float _y, float _z)
    {
        //Quaternion q;
        //q = m_CardFront.getLocalRotation();
        m_CardFront.rotate(_x, _y, _z);
        m_CardBack.rotate(_x, _y, _z);
    }

    public void move(Vector3f _vector)
    {
        m_CardFront.move(_vector);
        m_CardBack.move(_vector);
    }

    public void setLocalTranslation(Vector3f _vector)
    {
        m_Position = _vector.clone();
        m_CardFront.setLocalTranslation(_vector);
        m_CardBack.setLocalTranslation(_vector);
    }

    public void setLocalRotation(Vector3f _vector)
    {
        Quaternion q = new Quaternion(_vector.x, _vector.y, _vector.z, 1);
        m_CardFront.setLocalRotation(q);

        m_CardBack.setLocalRotation(q);
        m_CardBack.rotate(3.14f, 0, 0);
    }

    public void move(float _x, float _y, float _z)
    {
        m_CardFront.move(_x, _y, _z);
        m_CardBack.move(_x, _y, _z);
    }

    public void announcePos()
    {
        if (m_isAnnouced)
        {
            setLocalTranslation(m_Position);
            m_isAnnouced = false;
        } else
        {
            move(Positions.ANNOUNCE_MOTION);
            m_isAnnouced = true;
        }
    }
}
