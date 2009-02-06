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
	//################## CONSTRUCTORS ##################//
	public LightBrush(Point3d position)
	{
		// NYI
		throw new UnsupportedOperationException();
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