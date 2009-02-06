package MapEditor.Brushes;

import MapEditor.Commands.*;
import MapEditor.Geom.AxisAlignedBox;
import MapEditor.Geom.AxisPair;
import MapEditor.Graphics.IRenderer;
import MapEditor.Math.Vectors.*;
import MapEditor.Misc.Pair;
import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Stroke;
import java.io.PrintWriter;
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
	private LightBrush(Vector3d position, Color colour, boolean ghost, boolean isNew)
	{
		super(isNew);

		m_colour = colour;
		m_ghost = ghost;

		final double HALF_BOX_SIZE = 8;
		Vector3d mins = VectorUtil.subtract(position, new Vector3d(HALF_BOX_SIZE,HALF_BOX_SIZE,HALF_BOX_SIZE));
		Vector3d maxs = VectorUtil.add(position, new Vector3d(HALF_BOX_SIZE,HALF_BOX_SIZE,HALF_BOX_SIZE));
		m_boundingBox = new BoundingBox(new AxisAlignedBox(mins, maxs));
	}

	public LightBrush(Vector3d position)
	{
		this(position, Color.white, true, true);
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
		if(m_ghost)
		{
			// This Command must be here rather than in the BrushCommands class because
			// we're referencing LightBrush's private variable m_ghost, which isn't
			// accessible externally.
			CommandManager.instance().execute_compressible_command(new Command("Deghost")
			{
				public void execute()
				{
					// We've finished creating this brush.
					m_ghost = false;
					container.add_brush(LightBrush.this);
				}

				public void undo()
				{
					m_ghost = true;

					// Note that the selected brush is always PolyhedralBrush.this, since
					// at the point at which we call undo(), we're in the state resulting
					// from calling execute().
					container.ghost_selection();
				}
			},
			Pair.make_pair("Initial Brush Creation", "Brush Creation"));
		}
	}

	public boolean is_copyable()
	{
		return !is_ghost();
	}

	public boolean is_ghost()
	{
		return m_ghost;
	}

	public PickResults pick(final Vector3d start, final Vector3d direction)
	{
		// NYI
		return null;
	}

	public void render(IRenderer renderer, Color overrideColour)
	{
		if(overrideColour != null) renderer.set_colour(overrideColour);
		else renderer.set_colour(Color.yellow);

		float[] dash = {2.0f, 4.0f};
		Stroke stroke = new BasicStroke(1.0f, BasicStroke.CAP_SQUARE, BasicStroke.JOIN_MITER, 10.0f, dash, 0.0f);

		render_bounds(renderer, stroke);
		render_centre_cross(renderer);
	}

	public void render3D(GL gl, GLU glu, boolean bRenderNormals, boolean bRenderTextures)
	{
		// NYI
	}

	public void render_selected(IRenderer renderer, Color overrideColour)
	{
		if(overrideColour != null) renderer.set_colour(overrideColour);
		else if(m_ghost) renderer.set_colour(Color.white);
		else renderer.set_colour(Color.red);

		float[] dash = {2.0f, 4.0f};
		Stroke stroke = new BasicStroke(1.0f, BasicStroke.CAP_SQUARE, BasicStroke.JOIN_MITER, 10.0f, dash, 0.0f);

		render_bounds(renderer, stroke);
		render_centre_cross(renderer);
	}

	public void render3D_selected(GL gl, GLU glu, boolean bRenderNormals, boolean bRenderTextures)
	{
		// NYI
	}

	public void save_MEF2(PrintWriter pw)
	{
		// NYI
	}

	public double selection_metric(Vector2d p, IRenderer renderer)
	{
		// The metric is simple: return the square of the nearest distance to
		// the centre or to a bounding box edge.
		AxisPair ap = renderer.get_axis_pair();
		Vector2d centre = ap.select_components(m_boundingBox.centre());

		double bestMetric = renderer.distance_squared(p, centre);

		return bestMetric;
	}
}