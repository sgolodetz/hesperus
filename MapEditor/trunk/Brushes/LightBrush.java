package MapEditor.Brushes;

import MapEditor.Commands.*;
import MapEditor.Geom.AxisAlignedBox;
import MapEditor.Geom.AxisPair;
import MapEditor.Graphics.*;
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
	private LightBrush()		// for internal use
	{
		super(false);
	}

	private LightBrush(Vector3d position, Color colour, boolean ghost, boolean isNew)
	{
		super(isNew);

		m_colour = colour;
		m_ghost = ghost;

		final double HALF_BOX_SIZE = 16;
		Vector3d mins = VectorUtil.subtract(position, new Vector3d(HALF_BOX_SIZE,HALF_BOX_SIZE,HALF_BOX_SIZE));
		Vector3d maxs = VectorUtil.add(position, new Vector3d(HALF_BOX_SIZE,HALF_BOX_SIZE,HALF_BOX_SIZE));
		m_boundingBox = new BoundingBox(new AxisAlignedBox(mins, maxs));
	}

	public LightBrush(Vector3d position)
	{
		this(position, Color.white, true, true);
	}

	//################## PUBLIC METHODS ##################//
	public LightBrush copy()
	{
		LightBrush ret = new LightBrush();

		ret.m_boundingBox = m_boundingBox.clone();
		ret.m_colour = new Color(m_colour.getRGB());
		ret.m_ghost = false;

		return ret;
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

	public IBrush deselect(IBrushContainer container)
	{
		clear_state();
		return null;
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
		else renderer.set_colour(m_colour);

		render2D_light(renderer);
	}

	public void render3D(GL gl, GLU glu, boolean bRenderNormals, boolean bRenderTextures)
	{
		float[] colour = null;
		colour = m_colour.getRGBComponents(colour);
		gl.glColor3fv(colour);

		render3D_light(gl, glu);
	}

	public void render_selected(IRenderer renderer, Color overrideColour)
	{
		if(overrideColour != null) renderer.set_colour(overrideColour);
		else if(m_ghost) renderer.set_colour(Color.white);
		else renderer.set_colour(Color.red);

		render2D_light(renderer);
	}

	public void render3D_selected(GL gl, GLU glu, boolean bRenderNormals, boolean bRenderTextures)
	{
		gl.glColor3d(1,0,1);
		render3D_light(gl, glu);
	}

	public void save_MEF2(PrintWriter pw)
	{
		// NYI
	}

	public double selection_metric(Vector2d p, IRenderer renderer)
	{
		// The metric is simple: return the square of the nearest distance to the centre or the surrounding circle.
		AxisPair ap = renderer.get_axis_pair();
		Vector2d centre = ap.select_components(m_boundingBox.centre());

		Vector3d mins = m_boundingBox.get_bounds()[0], maxs = m_boundingBox.get_bounds()[1];
		double radius = renderer.pixel_distance(0, (maxs.x - mins.x) / 2);

		double squareDistToCentre = renderer.distance_squared(p, centre);
		double distToCentre = Math.sqrt(squareDistToCentre);
		double distToCircle = distToCentre - radius;
		double squareDistToCircle = distToCircle*distToCircle;

		return Math.min(squareDistToCentre, squareDistToCircle);
	}

	//################## PROTECTED METHODS ##################//
	protected void translate(Vector3d trans)
	{
		// It's easy to recalculate bounding boxes while we're translating, so we might as well
		// in order to make things easier for users.
		m_boundingBox = m_cachedBoundingBox.clone();
		m_boundingBox.translate(trans);
	}

	//################## PRIVATE METHODS ##################//
	private void render2D_light(IRenderer renderer)
	{
		float[] dash = {2.0f, 4.0f};
		Stroke stroke = new BasicStroke(1.0f, BasicStroke.CAP_SQUARE, BasicStroke.JOIN_MITER, 10.0f, dash, 0.0f);

		renderer.set_stroke(stroke);
		Vector3d mins = m_boundingBox.get_bounds()[0], maxs = m_boundingBox.get_bounds()[1];
		renderer.draw_oval(mins, maxs);
		renderer.set_stroke(new BasicStroke());

		render_centre_cross(renderer);
	}

	private void render3D_light(GL gl, GLU glu)
	{
		Vector3d centre = m_boundingBox.centre();
		GLUquadric quadric = glu.gluNewQuadric();
		gl.glPushMatrix();
			gl.glTranslated(centre.x, centre.y, centre.z);
			glu.gluSphere(quadric, 16.0f, 8, 8);
		gl.glPopMatrix();
		glu.gluDeleteQuadric(quadric);
	}
}