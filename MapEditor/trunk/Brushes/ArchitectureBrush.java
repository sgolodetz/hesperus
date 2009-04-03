package MapEditor.Brushes;

import MapEditor.Geom.AxisPair;
import MapEditor.Geom.Planar.*;
import MapEditor.Graphics.IRenderer;
import MapEditor.Math.Vectors.*;
import MapEditor.Misc.Pair;
import java.awt.*;
import java.awt.event.*;
import java.util.LinkedList;

/**
This is the base class for the polygonal brushes which represent the level architecture.
*/
public abstract class ArchitectureBrush extends TransformableBrush
{
	//################## CONSTRUCTORS ##################//
	/**
	Constructs an ArchitectureBrush whose initial state depends on whether or not it's a
	new brush. By "new", I mean one the user is creating, as opposed to one which we're
	loading in, one which is being constructed as a result of a split operation, etc.

	@param isNew	Is the brush a new one that the user is creating?
	*/
	public ArchitectureBrush(boolean isNew)
	{
		super(isNew);
	}

	//################## PUBLIC ABSTRACT METHODS ##################//
	/**
	Classifies the brush relative to the specified plane.

	@param plane	The plane relative to which to classify the brush
	@return			The classification, one of GeomConstants.{CP_BACK, CP_COPLANAR, CP_FRONT, CP_STRADDLE}
	*/
	public abstract int classify_against_plane(final Plane plane);

	/**
	Returns a list of all the brushes in the brush tree rooted at this brush. In essence, this
	is a tree-flattening function.

	@return	The component brushes as a LinkedList
	*/
	public abstract LinkedList<ArchitectureBrush> component_brushes();

	/**
	Returns a list of the faces of all the brushes in the brush tree rooted at this brush.

	@return	The faces as a LinkedList
	*/
	public abstract LinkedList<MapEditor.Geom.Planar.Polygon> faces();

	/**
	Reflects the brush across a plane.

	@param plane	The plane across which we are reflecting
	*/
	public abstract void reflect(Plane plane);

	/**
	Splits the brush using the specified split plane.

	<p><b>Preconditions:</b>
	<dl>
	<dd>The brush must straddle the plane
	<dd>The brush must not have a dimension equal to zero
	</dl>

	@param plane				The plane with which to split the brush
	@return						A pair of brushes, the first of which is the brush fragment in front of the
								splitting plane and the second of which is that behind it
	@throws java.lang.Error		If the preconditions aren't met
	*/
	public abstract Pair<ArchitectureBrush,ArchitectureBrush> split_with_plane(final Plane plane);

	/**
	Returns the polygons which will be used to generate the tree for this brush. In practice,
	what this means is that we return a list of polygon iterables, each of which corresponds
	to a single brush. This list of polygons will then have a CSG union operation performed on
	it before the final tree is generated from the surviving fragments.

	@return	A list of polygon iterables for tree generation, as specified, or null if no tree
			can be generated from this brush
	*/
	public abstract LinkedList<Iterable<MapEditor.Geom.Planar.Polygon>> tree_polygons();

	//################## PROTECTED ABSTRACT METHODS ##################//
	/**
	TODO
	*/
	//protected abstract Properties get_properties();

	/**
	TODO
	*/
	//protected abstract void set_properties(Properties properties);

	//################## PUBLIC METHODS ##################//
	/**
	Returns the centre of the brush.

	@return	The centre of the brush as a Vector3d
	*/
	final public Vector3d centre()
	{
		return m_boundingBox.centre();
	}

	public ArchitectureBrush copy()
	{
		// This is needed to override the return type to ArchitectureBrush instead of IBrush.
		throw new UnsupportedOperationException();
	}

	public IBrush deselect(IBrushContainer container)
	{
		clear_state();
		return null;
	}

	final public void flip_x()
	{
		reflect(new Plane(new Vector3d(1,0,0), centre().x));
	}

	final public void flip_y()
	{
		reflect(new Plane(new Vector3d(0,1,0), centre().y));
	}

	final public void flip_z()
	{
		reflect(new Plane(new Vector3d(0,0,1), centre().z));
	}

	final public boolean is_flippable()
	{
		return true;
	}

	public Dialog properties_dialog(Frame owner)
	{
		return new PropertiesDialog(owner);
	}

	//################## NESTED CLASSES ##################//
	private class PropertiesDialog extends Dialog
	{
		//################## CONSTRUCTORS ##################//
		public PropertiesDialog(Frame owner)
		{
			super(owner, "Architecture Brush Properties", true);

			addWindowListener(new WindowAdapter()
			{
				public void windowClosing(WindowEvent e)
				{
					dispose();
				}
			});

			setLayout(new BorderLayout(0, 5));

			Panel top = new Panel();
			top.setLayout(new GridLayout(3, 1));
			add("Center", top);

			Choice choice = new Choice();
			choice.add("Collision");
			choice.add("Detail");
			choice.add("Fog");
			choice.add("Hint");
			choice.add("Normal");
			choice.add("Water");
			top.add(choice);

			Panel bottom = new Panel();
			bottom.setLayout(new GridLayout(0, 2, 5, 0));
			add("South", bottom);

			Button okButton = new Button("OK");
			okButton.addActionListener(new ActionListener()
			{
				public void actionPerformed(ActionEvent e)
				{
					// TODO

					dispose();
				}
			});
			bottom.add(okButton);
			Button cancelButton = new Button("Cancel");
			cancelButton.addActionListener(new ActionListener()
			{
				public void actionPerformed(ActionEvent e)
				{
					dispose();
				}
			});
			bottom.add(cancelButton);

			pack();
			setSize(new Dimension(250, 150));
			setLocationRelativeTo(owner);		// centre the dialog relative to its owner
			setResizable(false);
			setVisible(true);
		}
	}
}