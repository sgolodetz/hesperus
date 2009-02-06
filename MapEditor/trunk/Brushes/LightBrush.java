package MapEditor.Brushes;

import MapEditor.Graphics.IRenderer;
import java.awt.Color;
import javax.vecmath.*;
import net.java.games.jogl.*;

/**
This class represents a point light brush.

FIXME: This isn't really an abstract class, it's just marked as abstract temporarily to make it compile.
*/
public class LightBrush extends TranslatableBrush
{
	//################## PRIVATE VARIABLES ##################//
	private Color m_colour;		// the RGB colour of the light
	private boolean m_ghost;	// are we in the process of creating this brush in the design canvas?
	
	//################## CONSTRUCTORS ##################//
	private LightBrush(Point3d position, Color colour, boolean isNew)
	{
		super(isNew);

		m_colour = colour;

		// TODO: Construct a suitably-sized bounding box.
	}

	public LightBrush(Point3d position)
	{
		this(position, Color.white, true);
	}

	//################## PROTECTED METHODS ##################//
	protected void translate(Point3d trans)
	{
		// NYI
		throw new UnsupportedOperationException();
	}

	//################## PUBLIC METHODS ##################//
	public LightBrush copy()
	{
		// NYI
		throw new UnsupportedOperationException();
	}

	public void deghost(final IBrushContainer container)
	{
		// NYI
		throw new UnsupportedOperationException();
	}

	public boolean is_copyable()
	{
		// NYI
		throw new UnsupportedOperationException();
	}

	public boolean is_ghost()
	{
		// NYI
		throw new UnsupportedOperationException();
	}

	public void render_selected(IRenderer renderer, Color overrideColour)
	{
		// NYI
		throw new UnsupportedOperationException();
	}

	public void render3D_selected(GL gl, GLU glu, boolean bRenderNormals, boolean bRenderTextures)
	{
		// NYI
		throw new UnsupportedOperationException();
	}

	public double selection_metric(Point2d p, IRenderer renderer)
	{
		// NYI
		throw new UnsupportedOperationException();
	}
}