package ChouineRenderer;

import Cards.Announce;
import com.jme3.app.SimpleApplication;
import com.jme3.material.Material;
import com.jme3.math.Vector3f;
import com.jme3.math.ColorRGBA;
import com.jme3.renderer.RenderManager;
import com.jme3.collision.CollisionResults;
import com.jme3.math.Vector2f;
import com.jme3.scene.Geometry;
import com.jme3.math.Ray;
import com.jme3.input.MouseInput;
import com.jme3.input.controls.ActionListener;
import com.jme3.input.controls.AnalogListener;
import com.jme3.input.controls.MouseAxisTrigger;
import com.jme3.input.controls.MouseButtonTrigger;
import com.jme3.input.controls.KeyTrigger;
import com.jme3.scene.Mesh;
import com.jme3.scene.debug.Arrow;
import com.jme3.scene.shape.Box;
import com.jme3.texture.Texture;
import ChouineAlgo.Chouine;
import com.jme3.asset.TextureKey;
import com.jme3.input.KeyInput;
import com.jme3.niftygui.NiftyJmeDisplay;
import com.jme3.system.AppSettings;
import com.jme3.system.JmeSystem;
import de.lessvoid.nifty.Nifty;
import de.lessvoid.nifty.screen.Screen;
import de.lessvoid.nifty.screen.ScreenController;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.security.NoSuchAlgorithmException;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.Callable;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.imageio.ImageIO;
import javax.swing.JFrame;

/**
 * test
 *
 * @author normenhansen
 */
public class Main extends SimpleApplication implements ScreenController
{
    // test vectors
    public static boolean TEST=false;
    public static int TESTID=1;
    
    public static LinkedList<Integer[]> TESTVECTORS = new LinkedList<Integer[]>();
    public void CreateTests()
    {
        // Order original des cartes
        // Carreau (7(id:0) -> as(7)), coeur(7(8) -> as(15)), pic(7(16) -> as(23), trefle(7(24) -> as(31))
        
        TESTVECTORS.add(new Integer[] {3, 17, 4, 18, 5, 19, 6, 20, 7, 21, 8});   // 0
        TESTVECTORS.add(new Integer[] {0, 6, 1, 7, 2, 14, 8, 15, 9, 22, 10, 23});  // 1
    }
    //
    // OPTIONS
    //
    public static boolean SHOW_COMPUTER_CARDS = false;

    public enum STATE
    {
        NONE, PLAY, USER_ANNOUNCE, GAMEOVER
    }
    Geometry m_Carpet;
    Geometry selectedCard;
    boolean m_MoveUserWinCards;
    boolean m_MoveComputerWinCards;
    CardManager m_CardManager;
    ControlScreen m_ControlScreen;
    Chouine m_Chouine;
    Vector3f m_CamPosition;
    Timer m_RemoveCardsTimer;
    float m_CamAngle;
    private static final int RESOL_X = 1600;
    private static final int RESOL_Y = 900;
    static Main mainApp;
    int m_PlayerId;
    int m_ComputerId;
    int m_WinnerId;
    private Nifty nifty;
    private STATE m_State;
    Timer localtimer;
    private boolean initDone;
    static boolean RemoveDone;

    public static void main(String[] args)
    {
        try {
            if (System.getProperty("javawebstart.version") != null)
            {
                JmeSystem.setLowPermissions(true);
            }
            
            BufferedImage[] icons = new BufferedImage[]
            {
                ImageIO.read(Main.class.getResource("chouine_32.png"))
            };

            AppSettings settings = new AppSettings(true);
            settings.setFrameRate(15);
            settings.setResolution(RESOL_X, RESOL_Y);
            String title = "Chouine V1.2";
            if (TEST)
            {
                title += " !!! MODE TEST !!!";
            }
            settings.setTitle(title);
            settings.setIcons(icons);
            
            RemoveDone = false;
            
            /*JFrame rules = new JFrame();
            rules.setVisible(true);
            rules.setTitle("Règle du jeu de la Chouine");
            rules.setLocationRelativeTo(null);
            rules.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
*/
            Main app = new Main();
            mainApp = app;
            app.setDisplayStatView(false);
            app.setDisplayFps(false);
            app.setShowSettings(false);
            app.setSettings(settings);
            app.start();

            app.killTimer();
            //app.stop();
        } catch (IOException ex) {
            Logger.getLogger(Main.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

   
    
    public void setState(STATE _state)
    {
        m_State = _state;
    }

    public STATE getState()
    {
        return m_State;
    }

    public void killTimer()
    {
        if (m_RemoveCardsTimer != null)
        {
            m_RemoveCardsTimer.cancel();
            m_RemoveCardsTimer.purge();
            m_RemoveCardsTimer = null;
        }
    }

    @Override
    public void simpleInitApp()
    {
        CreateTests();
        m_Carpet = null;
        initDone = false;
        m_MoveUserWinCards = false;
        m_MoveComputerWinCards = false;
        m_CamAngle = Positions.CAM_ANGLE;
        m_CamPosition = new Vector3f(Positions.CAM_X,
                Positions.CAM_Y,
                Positions.CAM_Z);
        m_CamPosition.z = (float) (Positions.CAM_Z * Math.cos(m_CamAngle));
        m_CamPosition.y = (float) (Positions.CAM_Z * Math.sin(m_CamAngle));

        m_CardManager = new CardManager(assetManager, rootNode);
        m_Chouine = new Chouine();

        if (TEST)
        {
            SHOW_COMPUTER_CARDS = true;
        }
        m_State = STATE.NONE;
        NiftyJmeDisplay niftyDisplay = new NiftyJmeDisplay(assetManager,
                inputManager,
                audioRenderer,
                guiViewPort);
        nifty = niftyDisplay.getNifty();
        m_ControlScreen = new ChouineRenderer.ControlScreen(this, m_CardManager, m_Chouine);
        nifty.fromXml("Interface/StartGui.xml", "start",
                m_ControlScreen);
        //nifty.setDebugOptionPanelColors(true);

        // attach the nifty display to the gui view port as a processor
        guiViewPort.addProcessor(niftyDisplay);
        flyCam.setEnabled(false);
        inputManager.setCursorVisible(true);
        setDisplayStatView(false);
        //setDisplayFps(false);
    }

    public void clear()
    {
        m_CardManager.init();
        rootNode.detachChild(m_Carpet);
    }

    public void initGame() throws NoSuchAlgorithmException
    {
        // init new game  
        m_CardManager.init();
        createCarpet();
        m_RemoveCardsTimer = null;
        LinkedList<Integer> order = null;

        m_PlayerId = CardManager.PLAYER_ID;
        m_ComputerId = CardManager.COMPUTER_ID;

        //m_Chouine.setPlayerAlgo(m_PlayerId, 1);
        if (TEST)
        {
            order = new LinkedList<Integer>(Arrays.asList(TESTVECTORS.get(TESTID)));
        }
        m_Chouine.NewGame("Computer", "Joueur", order);

        m_Chouine.setPlayerLevel(m_ComputerId, Options.getComputerLevel());

        m_CardManager.SetPickCards(m_Chouine.GetPickList());
        m_CardManager.RemovePickCard(10);

        m_Chouine.DistributeCards(!TEST); // no random when testing

        m_CardManager.SetUserCards(m_Chouine.GetPlayerCardList(m_PlayerId));
        m_CardManager.SetComputerCards(m_Chouine.GetPlayerCardList(m_ComputerId));

        selectedCard = null;

        Cards.Card c = m_Chouine.GetTrumpCard();
        m_ControlScreen.trumpCardName(c.colorName());

        //attachCoordinateAxes(new Vector3f(0, 0, 0));
        // start game
        if (m_Chouine.donnor() == m_ComputerId)
        {
            ComputerChoice();
        } else
        {
            m_ControlScreen.newMessage("A vous de jouer...");
        }

        if (initDone == false)
        {
            init();
        }
        m_State = STATE.PLAY;

        if (Options.getShowCardToPlay())
        {
            Cards.Card userChoice = m_Chouine.getPlayerChoice(CardManager.PLAYER_ID, true);
            m_CardManager.showComputerChoice(userChoice.displayName());
        }
        
        m_ControlScreen.updateComputerScore(
                    m_Chouine.getPlayerPoints(m_ComputerId),
                    m_Chouine.getPlayerAnnouncesPoints(m_ComputerId));
        m_ControlScreen.updateUserScore(
                    m_Chouine.getPlayerPoints(m_PlayerId),
                    m_Chouine.getPlayerAnnouncesPoints(m_PlayerId));

    }

    @Override
    public void simpleUpdate(float tpf)
    {
        if (RemoveDone == true)
        {
            RemoveDone = false;
            updateAfterDone();
        }
    }

    @Override
    public void simpleRender(RenderManager rm)
    {
        //TODO: add render code
    }

    private void createCarpet()
    {
        if (m_Carpet == null)
        {
            Box b = new Box(Positions.CARPET_HEIGHT / 2,
                    Positions.CARPET_WIDTH / 2,
                    Positions.CARPET_TICK / 2);
            m_Carpet = new Geometry("Carpet", b);
            Material mat = new Material(assetManager, "Common/MatDefs/Misc/Unshaded.j3md");
            mat.setColor("Color", Positions.CARPET_COLOR);
            
            Texture tex = assetManager.loadTexture( new TextureKey("Textures/tapis.JPG"));
            
            tex.setWrap(Texture.WrapMode.Repeat);
            mat.setTexture("ColorMap", tex);
                            //assetManager.loadTexture("Textures/tapis.JPG"));
            ((Geometry)(m_Carpet)).getMesh().scaleTextureCoordinates(new Vector2f(100,100));
            m_Carpet.setMaterial(mat);
            m_Carpet.move(Positions.CARPET_POS);
        }

        rootNode.attachChild(m_Carpet);
    }

    private void init()
    {
        initKeys();
        initDone = true;
    }

    private boolean ComputerChoice()
    {
        Cards.Card card;
        Announce ann;
        boolean isAnnounce = false;
        String name;

        card = m_Chouine.getPlayerChoice(m_ComputerId, false);
        ann = m_Chouine.getPlayerLatestAnnounce(m_ComputerId);
        if (ann != null)
        {
            String message;
            message = "Ordinateur : " + ann.getName();
            if (ann.isTrump())
            {
                message += " d'atout";
            }
            if (ann.getType() != Announce.AnnounceType.CHOUINE)
            {
               message += " : " + String.valueOf(ann.getPoints()) + " points";
            }
            m_ControlScreen.newMessage(message);
            m_ControlScreen.updateComputerScore(
                    m_Chouine.getPlayerPoints(m_ComputerId),
                    m_Chouine.getPlayerAnnouncesPoints(m_ComputerId));
            isAnnounce = true;
        }
        name = m_Chouine.hasChange7Trump(m_ComputerId);
        if (name != null)
        {
            m_CardManager.exchangeTrumpCard(CardManager.COMPUTER_ID, name);
        }

        if (card != null)
        {
            m_CardManager.SetComputerCard(card);
        }
        
        if (m_Chouine.isGameOver())
        {
            clear();
            displayResults();
        } 
        
        return isAnnounce;
    }

    private void attachCoordinateAxes(Vector3f pos)
    {
        Arrow arrow = new Arrow(Vector3f.UNIT_X);
        arrow.setLineWidth(4); // make arrow thicker
        putShape(arrow, ColorRGBA.Red).setLocalTranslation(pos);

        arrow = new Arrow(Vector3f.UNIT_Y);
        arrow.setLineWidth(4); // make arrow thicker
        putShape(arrow, ColorRGBA.Green).setLocalTranslation(pos);

        arrow = new Arrow(Vector3f.UNIT_Z);
        arrow.setLineWidth(4); // make arrow thicker
        putShape(arrow, ColorRGBA.Blue).setLocalTranslation(pos);
    }

    private Geometry putShape(Mesh shape, ColorRGBA color)
    {
        Geometry g = new Geometry("coordinate axis", shape);
        Material mat = new Material(assetManager, "Common/MatDefs/Misc/Unshaded.j3md");
        mat.getAdditionalRenderState().setWireframe(true);
        mat.setColor("Color", color);
        g.setMaterial(mat);
        rootNode.attachChild(g);
        return g;
    }

    /**
     * Custom Keybinding: Map named actions to inputs.
     */
    private void initKeys()
    {
        flyCam.setEnabled(false);
        cam.setLocation(m_CamPosition);
        cam.lookAt(Positions.CAM_LOOKAT, Vector3f.UNIT_Z);

        Vector3f v;
        v = cam.getDirection();

        // You can map one or several inputs to one named action
        inputManager.setCursorVisible(true);

        inputManager.addMapping("Left", new MouseAxisTrigger(MouseInput.AXIS_X, false));
        inputManager.addMapping("Right", new MouseAxisTrigger(MouseInput.AXIS_X, true));
        inputManager.addMapping("Up", new MouseAxisTrigger(MouseInput.AXIS_Y, true));
        inputManager.addMapping("Down", new MouseAxisTrigger(MouseInput.AXIS_Y, false));
        inputManager.addMapping("Pick card", new MouseButtonTrigger(MouseInput.BUTTON_LEFT));

        /*inputManager.addMapping("Zoom+", new KeyTrigger(KeyInput.KEY_UP));
        inputManager.addMapping("Zoom-", new KeyTrigger(KeyInput.KEY_DOWN));
        inputManager.addMapping("RotateUp", new KeyTrigger(KeyInput.KEY_RIGHT));
        inputManager.addMapping("RotateDown", new KeyTrigger(KeyInput.KEY_LEFT));
*/
        inputManager.addMapping("C", new KeyTrigger(KeyInput.KEY_C));

        // Add the names to the action listener.
        //inputManager.addListener(combinedListener, "Zoom+", "Zoom-", "RotateUp", "RotateDown");
        inputManager.addListener(combinedListener, "Left", "Right", "Up", "Down", "Pick card");
        inputManager.addListener(combinedListener, "C");
        //inputManager.reset();
    }

    public void displayResults()
    {
        int comp, user;
        
        m_State = STATE.GAMEOVER;
        m_CardManager.SetUserWinCards(m_Chouine.GetPlayerWinCardList(m_PlayerId));
        m_CardManager.SetComputerWinCards(m_Chouine.GetPlayerWinCardList(m_ComputerId));

        comp = m_Chouine.getPlayerTotalPoints(m_ComputerId);
        user = m_Chouine.getPlayerTotalPoints(m_PlayerId);
        

        m_ControlScreen.gameOverScreen(user, comp);
    }

    public void updateAfterDone()
    {
        // remove card in the middle of the table
        m_CardManager.RemovePlayedCards(m_WinnerId);

        // update user cards
        m_CardManager.SetUserCards(m_Chouine.GetPlayerCardList(m_PlayerId));
        m_CardManager.SetComputerCards(m_Chouine.GetPlayerCardList(m_ComputerId));

        // remove cards for the pick
        m_CardManager.RemovePickCard(2);

        m_ControlScreen.updateComputerScore(
                m_Chouine.getPlayerPoints(m_ComputerId),
                m_Chouine.getPlayerAnnouncesPoints(m_ComputerId));

        m_ControlScreen.updateUserScore(
                m_Chouine.getPlayerPoints(m_PlayerId),
                m_Chouine.getPlayerAnnouncesPoints(m_PlayerId));

        m_ControlScreen.changePickCardLeft(m_Chouine.GetPickList().size());
        //m_CardManager.SetComputerCards(m_Chouine.GetComputerCardList());

        // if this is the computer to play, wait a while before displaying the card
        if (m_WinnerId == CardManager.COMPUTER_ID)
        {
            try
            {
                Thread.sleep((long) ((Options.getRemoveCardsTimeOut() / 2) * 1000));
            } catch (InterruptedException ex)
            {
                Logger.getLogger(Main.class.getName()).log(Level.SEVERE, null, ex);
            }
            if (!m_Chouine.isGameOver())
            {
                ComputerChoice();
            }
        }
        
        // TEST
        //displayResults();
        
        if (m_Chouine.isGameOver())
        {
            displayResults();
        } else if (Options.getShowCardToPlay())
        {
            Cards.Card userChoice = m_Chouine.getPlayerChoice(CardManager.PLAYER_ID, true);
            m_CardManager.showComputerChoice(userChoice.displayName());
        }
        m_RemoveCardsTimer.cancel();
        m_RemoveCardsTimer.purge();
    }
   
       
    private MyCombinedListener combinedListener = new MyCombinedListener();

    private class MyCombinedListener implements AnalogListener, ActionListener
    {

        Main m_Main;

        public void SetMain(Main _main)
        {
            m_Main = _main;
        }

        private void takeCard()
        {
            if ((selectedCard != null) || (m_MoveUserWinCards == true) || (m_MoveComputerWinCards == true))
            {
                return;
            }
            selectedCard = null;
            // Reset results list.
            CollisionResults results = new CollisionResults();
            // Convert screen click to 3d position
            Vector2f click2d = inputManager.getCursorPosition();
            Vector3f click3d = cam.getWorldCoordinates(new Vector2f(click2d.x, click2d.y), -1f).clone();
            Vector3f dir = cam.getWorldCoordinates(new Vector2f(click2d.x, click2d.y), 1f).subtractLocal(click3d).normalizeLocal();
            // Aim the ray from the clicked spot forwards.
            Ray ray = new Ray(click3d, dir);
            // Collect intersections between ray and all nodes in results list.
            rootNode.collideWith(ray, results);
            //System.out.println("Object trouvés : " + results.size());
            if (results.size() > 0)
            {
                int i = 0;
                while ((i < results.size())
                        && ((!m_CardManager.IsMoveable(results.getCollision(i).getGeometry().getName()))) &&
                             (!m_CardManager.IsUserWinCard(results.getCollision(i).getGeometry().getName()))  &&
                             (!m_CardManager.IsComputerWinCard(results.getCollision(i).getGeometry().getName())))
                {
                    i++;
                }


                //System.out.println("Object : "+ String.valueOf(i) + "/" + String.valueOf(results.size()));
                if (i < results.size())
                {
                    if (m_CardManager.IsUserWinCard(results.getCollision(i).getGeometry().getName()))
                    {
                        m_MoveUserWinCards = true;
                        m_CardManager.ShowUserWinCard(true);
                    }
                    else
                    if (m_CardManager.IsComputerWinCard(results.getCollision(i).getGeometry().getName()))
                    {
                        m_MoveComputerWinCards = true;
                        m_CardManager.ShowComputerWinCard(true);
                    } else
                    if (m_CardManager.IsMoveable(results.getCollision(i).getGeometry().getName()))
                    {
                        selectedCard = results.getCollision(i).getGeometry();
                        //System.out.println("Carte selectionnée : " + selectedCard.getName());
                    }
                }
            }
        }

        private void playCard(Geometry _object) throws InterruptedException
        {
            int extraTime = 0;
            Vector3f pos = _object.getWorldTranslation();
            if (pos.y > Positions.CARD_ISPLAYED_LIMIT_Y)
            {
                Cards.Card card = null;
                boolean cardAllowed;

                cardAllowed = m_Chouine.setPlayerChoice(m_PlayerId, _object.getName());
                if (cardAllowed)
                {
                    m_CardManager.SetUserChoice(_object.getName());
                    if (m_Chouine.donnor() == m_PlayerId)
                    {
                        // display the card played by computer
                        if (ComputerChoice())
                        {
                            // computer has an announce, take more time before removing cards
                            extraTime = 1;
                        }
                    }
                    m_WinnerId = m_Chouine.Play();
                    
                    killTimer();
                    m_RemoveCardsTimer = new Timer();
                    m_RemoveCardsTimer.schedule(new TimerTask()
                    {
                        @Override
                        public void run()
                        {
                            RemoveDone = true;
                            //enqueueUpdateAfterDone();
                        }
                    }, (long) ((Options.getRemoveCardsTimeOut() + extraTime) * 1000));
                    
                } else
                {
                    m_CardManager.gotoOriginalPosition(_object.getName());
                    m_ControlScreen.newMessage("Veuillez respecter les règles");
                }

            }
        }

        private void announceCard(Geometry _object)
        {
            m_CardManager.announceCard(_object.getName());
        }

        public void onAction(String name, boolean keyPressed, float tpf)
        {
            Geometry object;

            if (name.equals("Pick card"))
            {
                if ((!keyPressed) && (selectedCard != null))
                {
                    object = selectedCard.clone();
                    selectedCard = null;
                    if (m_State == STATE.PLAY)
                    {
                        try {
                            playCard(object);
                        } catch (InterruptedException ex) {
                            Logger.getLogger(Main.class.getName()).log(Level.SEVERE, null, ex);
                        }
                    }
                    if (m_State == STATE.USER_ANNOUNCE)
                    {
                        announceCard(object);
                    }
                } else if ((!keyPressed) && (m_MoveUserWinCards == true))
                {
                    m_MoveUserWinCards = false;
                    m_CardManager.ShowUserWinCard(false);
                } else
                if ((!keyPressed) && (m_MoveComputerWinCards == true))
                {
                    m_MoveComputerWinCards = false;
                    m_CardManager.ShowComputerWinCard(false);
                    
                }
                else
                {
                    // usrer clicks the button and no card is selected
                    takeCard();
                }
            }
        }

        public void onAnalog(String name, float value, float tpf)
        {
            /*if ((name.equals("Zoom+")) && (m_CamPosition.z > Positions.ZOOM_Y_MAX))
            {
                double zPos = m_CamPosition.z / Math.cos(m_CamAngle);

                zPos -= Positions.ZOOM_SPEED;
                m_CamPosition.z = (float) (zPos * Math.cos(m_CamAngle));
                m_CamPosition.y = (float) (zPos * Math.sin(m_CamAngle));

                cam.setLocation(m_CamPosition);
                cam.lookAt(Positions.CAM_LOOKAT, Vector3f.UNIT_Z);
            }
            if ((name.equals("Zoom-")) && (m_CamPosition.z < Positions.ZOOM_Y_MIN))
            {
                double zPos = m_CamPosition.z / Math.cos(m_CamAngle);

                zPos += Positions.ZOOM_SPEED;
                m_CamPosition.z = (float) (zPos * Math.cos(m_CamAngle));
                m_CamPosition.y = (float) (zPos * Math.sin(m_CamAngle));

                cam.setLocation(m_CamPosition);
                cam.lookAt(Positions.CAM_LOOKAT, Vector3f.UNIT_Z);
            }

            if ((name.equals("RotateDown")) && (m_CamAngle > -1.30))
            {
                // recompute position with an null angle
                double zPos = m_CamPosition.z / Math.cos(m_CamAngle);

                m_CamAngle -= Positions.ROTATION_SEEP;

                m_CamPosition.z = (float) (zPos * Math.cos(m_CamAngle));
                m_CamPosition.y = (float) (zPos * Math.sin(m_CamAngle));
                cam.setLocation(m_CamPosition);
                cam.lookAt(Positions.CAM_LOOKAT, Vector3f.UNIT_Z);
            }
            if ((name.equals("RotateUp")) && (m_CamAngle < 0))
            {
                // recompute position with an null angle
                double zPos = m_CamPosition.z / Math.cos(m_CamAngle);

                m_CamAngle += Positions.ROTATION_SEEP;
                if (m_CamAngle > 0)
                {
                    m_CamAngle = 0;
                }
                m_CamPosition.z = (float) (zPos * Math.cos(m_CamAngle));
                m_CamPosition.y = (float) (zPos * Math.sin(m_CamAngle));
                cam.setLocation(m_CamPosition);
                cam.lookAt(Positions.CAM_LOOKAT, Vector3f.UNIT_Z);
            }*/

            if ((selectedCard != null) && (m_State == STATE.PLAY))
            {
                if (name.equals("C"))
                {
                    // show cards
                    if (SHOW_COMPUTER_CARDS)
                    {
                        SHOW_COMPUTER_CARDS = false;
                    }
                    else
                    {
                        SHOW_COMPUTER_CARDS = true;
                    }
                    //m_CardManager.RotateComputerCards();
                    m_CardManager.ShowComputerCards(!SHOW_COMPUTER_CARDS);
                }
                speed = 1.7f;
                if (name.equals("Right"))
                {
                    Vector3f v = selectedCard.getLocalTranslation();
                    selectedCard.setLocalTranslation(v.x - value * speed, v.y, v.z);
                    if (rootNode.getChild(selectedCard.getName() + "b") != null)
                    {
                        rootNode.getChild(selectedCard.getName() + "b").setLocalTranslation(v.x - value * speed, v.y, v.z);
                    }
                }
                if (name.equals("Left"))
                {
                    Vector3f v = selectedCard.getLocalTranslation();
                    selectedCard.setLocalTranslation(v.x + value * speed, v.y, v.z);
                    if (rootNode.getChild(selectedCard.getName() + "b") != null)
                    {
                        rootNode.getChild(selectedCard.getName() + "b").setLocalTranslation(v.x + value * speed, v.y, v.z);
                    }
                }
                if (name.equals("Up"))
                {
                    Vector3f v = selectedCard.getLocalTranslation();
                    selectedCard.setLocalTranslation(v.x, v.y - value * speed, v.z);
                    if (rootNode.getChild(selectedCard.getName() + "b") != null)
                    {
                        rootNode.getChild(selectedCard.getName() + "b").setLocalTranslation(v.x, v.y - value * speed, v.z);
                    }
                }
                if (name.equals("Down"))
                {
                    Vector3f v = selectedCard.getLocalTranslation();
                    selectedCard.setLocalTranslation(v.x, v.y + value * speed, v.z);
                    if (rootNode.getChild(selectedCard.getName() + "b") != null)
                    {
                        rootNode.getChild(selectedCard.getName() + "b").setLocalTranslation(v.x, v.y + value * speed, v.z);
                    }
                }
            }
            
            if ((m_MoveUserWinCards == true) && (m_State == STATE.PLAY))
            {
                speed = 2;
                if (name.equals("Right"))
                {
                    m_CardManager.MoveUserWinCard(-value*speed, 0);
                }
                if (name.equals("Left"))
                {
                    m_CardManager.MoveUserWinCard(value*speed, 0);
                }
                if (name.equals("Up"))
                {
                    m_CardManager.MoveUserWinCard(0, -value*speed);
                }
                if (name.equals("Down"))
                {
                    m_CardManager.MoveUserWinCard(0, value*speed);
                }
            }
            
            if ((m_MoveComputerWinCards == true) && (m_State == STATE.PLAY))
            {
                speed = 2;
                if (name.equals("Right"))
                {
                    m_CardManager.MoveComputerWinCard(-value*speed, 0);
                }
                if (name.equals("Left"))
                {
                    m_CardManager.MoveComputerWinCard(value*speed, 0);
                }
                if (name.equals("Up"))
                {
                    m_CardManager.MoveComputerWinCard(0, -value*speed);
                }
                if (name.equals("Down"))
                {
                    m_CardManager.MoveComputerWinCard(0, value*speed);
                }
            }

        }
    };

    public void startAnnounce()
    {
        m_State = STATE.USER_ANNOUNCE;
    }

    public void bind(Nifty nifty, Screen screen)
    {
        System.out.println("bind( " + screen.getScreenId() + ")");
    }

    public void onStartScreen()
    {
        System.out.println("onStartScreen");
    }

    public void onEndScreen()
    {
        System.out.println("onEndScreen");
    }

    public void quit()
    {
        nifty.gotoScreen("end");
    }
}
