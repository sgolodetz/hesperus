package MapEditor.Brushes;

import MapEditor.Graphics.IRenderer;
import MapEditor.Math.Vectors.*;
import java.awt.Color;
import net.java.games.jogl.*;

/**
This class represents a point light brush.
*/
public class LightBrush extends TranslatableBrush
{
	//################## PRIVATE VARIABLES ##################//
	private Color m_colour;		// the RGB colour of the light
	private boolean m_ghost;	// are we in the process of creating this brush in the design canvas?
	
	//################## CONSTRUCTORS ##################//
	private LightBrush(Vector3d position, Color colour, boolean isNew)
	{
		super(isNew);

		m_colour = colour;

		// TODO: Construct a suitably-sized bounding box.
	}

	public LightBrush(Vector3d position)
	{
		this(position, Color.white, true);
	}

	//################## PROTECTED METHODS ##################//
	protected void translate(Vector3d trans)
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

	public double selection_metric(Vector2d p, IRenderer renderer)
	{
		// NYI
		throw new UnsupportedOperationException();
	}
}