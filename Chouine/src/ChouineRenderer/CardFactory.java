/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ChouineRenderer;

import com.jme3.asset.AssetManager;
import com.jme3.material.Material;
import com.jme3.scene.Geometry;
import com.jme3.scene.Node;
import com.jme3.scene.shape.Quad;

/**
 *
 * @author vincentc
 */
public class CardFactory
{   
    Node rootNode;
    AssetManager assetManager;
        
    public CardFactory(AssetManager _manager, Node _node)
    {
        rootNode = _node;
        assetManager = _manager;
    }
    
    public Card createCard(String _cardName, Cards.Card.Color _color, int _value)
    {
        Quad quad = new Quad(Positions.CARD_WIDTH, Positions.CARD_HEIGHT);
        Geometry cardf = new Geometry(_cardName, quad);
        Geometry cardb = new Geometry(_cardName+"b", quad);

        Material matf = new Material(assetManager, "Common/MatDefs/Misc/Unshaded.j3md");
        Material matb = new Material(assetManager, "Common/MatDefs/Misc/Unshaded.j3md");

        switch (_color)
        {
            case CARREAU:
                switch (_value)
                {
                    case 0:
                        matf.setTexture("ColorMap",
                            assetManager.loadTexture("Textures/208px-07_of_diamonds.svg.png"));
                        break;
                    case 1:
                        matf.setTexture("ColorMap",
                            assetManager.loadTexture("Textures/208px-08_of_diamonds.svg.png"));
                        break;
                    case 2:
                        matf.setTexture("ColorMap",
                            assetManager.loadTexture("Textures/208px-09_of_diamonds.svg.png"));
                        break;
                    case 3:
                        matf.setTexture("ColorMap",
                            assetManager.loadTexture("Textures/209px-Jack_of_diamonds_fr.svg.png"));
                        break;
                    case 4:
                        matf.setTexture("ColorMap",
                            assetManager.loadTexture("Textures/209px-Queen_of_diamonds_fr.svg.png"));
                        break;
                    case 5:
                        matf.setTexture("ColorMap",
                            assetManager.loadTexture("Textures/209px-King_of_diamonds_fr.svg.png"));
                        break;
                    case 6:
                        matf.setTexture("ColorMap",
                            assetManager.loadTexture("Textures/208px-10_of_diamonds.svg.png"));
                        break;
                    case 7:
                        matf.setTexture("ColorMap",
                            assetManager.loadTexture("Textures/208px-01_of_diamonds_A.svg.png"));
                        break;
                }
                break;
            case PIC:
                switch (_value)
                {
                    case 0:
                        matf.setTexture("ColorMap",
                            assetManager.loadTexture("Textures/208px-07_of_spades.svg.png"));
                        break;
                    case 1:
                        matf.setTexture("ColorMap",
                            assetManager.loadTexture("Textures/208px-08_of_spades.svg.png"));
                        break;
                    case 2:
                        matf.setTexture("ColorMap",
                            assetManager.loadTexture("Textures/208px-09_of_spades.svg.png"));
                        break;
                    case 3:
                        matf.setTexture("ColorMap",
                            assetManager.loadTexture("Textures/209px-Jack_of_spades_fr.svg.png"));
                        break;
                    case 4:
                        matf.setTexture("ColorMap",
                            assetManager.loadTexture("Textures/209px-Queen_of_spades_fr.svg.png"));
                        break;
                    case 5:
                        matf.setTexture("ColorMap",
                            assetManager.loadTexture("Textures/209px-King_of_spades_fr.svg.png"));
                        break;
                    case 6:
                        matf.setTexture("ColorMap",
                            assetManager.loadTexture("Textures/208px-10_of_spades.svg.png"));
                        break;
                    case 7:
                        matf.setTexture("ColorMap",
                            assetManager.loadTexture("Textures/208px-01_of_spades_A.svg.png"));
                        break;
                }
                break;
            case COEUR:
                switch (_value)
                {
                    case 0:
                        matf.setTexture("ColorMap",
                            assetManager.loadTexture("Textures/208px-07_of_hearts.svg.png"));
                        break;
                    case 1:
                        matf.setTexture("ColorMap",
                            assetManager.loadTexture("Textures/208px-08_of_hearts.svg.png"));
                        break;
                    case 2:
                        matf.setTexture("ColorMap",
                            assetManager.loadTexture("Textures/208px-09_of_hearts.svg.png"));
                        break;
                    case 3:
                        matf.setTexture("ColorMap",
                            assetManager.loadTexture("Textures/209px-Jack_of_hearts_fr.svg.png"));
                        break;
                    case 4:
                        matf.setTexture("ColorMap",
                            assetManager.loadTexture("Textures/209px-Queen_of_hearts_fr.svg.png"));
                        break;
                    case 5:
                        matf.setTexture("ColorMap",
                            assetManager.loadTexture("Textures/209px-King_of_hearts_fr.svg.png"));
                        break;
                    case 6:
                        matf.setTexture("ColorMap",
                            assetManager.loadTexture("Textures/208px-10_of_hearts.svg.png"));
                        break;
                    case 7:
                        matf.setTexture("ColorMap",
                            assetManager.loadTexture("Textures/208px-01_of_hearts_A.svg.png"));
                        break;
                }
                break;
            case TREFLE:
                switch (_value)
                {
                    case 0:
                        matf.setTexture("ColorMap",
                            assetManager.loadTexture("Textures/208px-07_of_clubs.svg.png"));
                        break;
                    case 1:
                        matf.setTexture("ColorMap",
                            assetManager.loadTexture("Textures/208px-08_of_clubs.svg.png"));
                        break;
                    case 2:
                        matf.setTexture("ColorMap",
                            assetManager.loadTexture("Textures/208px-09_of_clubs.svg.png"));
                        break;
                    case 3:
                        matf.setTexture("ColorMap",
                            assetManager.loadTexture("Textures/209px-Jack_of_clubs_fr.svg.png"));
                        break;
                    case 4:
                        matf.setTexture("ColorMap",
                            assetManager.loadTexture("Textures/209px-Queen_of_clubs_fr.svg.png"));
                        break;
                    case 5:
                        matf.setTexture("ColorMap",
                            assetManager.loadTexture("Textures/209px-King_of_clubs_fr.svg.png"));
                        break;
                    case 6:
                        matf.setTexture("ColorMap",
                            assetManager.loadTexture("Textures/208px-10_of_clubs.svg.png"));
                        break;
                    case 7:
                        matf.setTexture("ColorMap",
                            assetManager.loadTexture("Textures/208px-01_of_clubs_A.svg.png"));
                        break;
                }
                break;
            default: // toto produce an error
                break;
        }
        
        matb.setTexture("ColorMap",
                            assetManager.loadTexture("Textures/cardback_chouine.png"));
        
        
matf.getAdditionalRenderState().setAlphaTest(true);
//matb.getAdditionalRenderState().setAlphaTest(true);
        cardf.setMaterial(matf);
        cardb.setMaterial(matb);
        
        cardb.rotate(3.14f, 0, 0);
        
        rootNode.attachChild(cardf);
        rootNode.attachChild(cardb);
        
        return new Card(cardf, cardb);
    }
}
