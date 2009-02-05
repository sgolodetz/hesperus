package MapEditor.Brushes;

import MapEditor.Commands.*;
import MapEditor.Geom.AxisAlignedBox;
import MapEditor.Geom.AxisPair;
import MapEditor.Graphics.IRenderer;
import MapEditor.Math.MathUtil;
import MapEditor.Misc.*;
import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Cursor;
import java.awt.Stroke;
import javax.vecmath.*;

/**
This is the base class for brushes which can be resized and translated.
*/
public abstract class ResizableTranslatableBrush extends BrushAdapter implements BrushConstants, Constants
{
	//################## PROTECTED ENUMERATIONS ##################//
	protected enum Axis
	{
		NONE, X, Y
	}

	protected enum HandleState
	{
		RESIZE, ROTATE, SHEAR
	}

	protected enum State
	{
		IDLE,				// no operation is active on the brush (initial state for internally created brushes)
		INITIAL,			// the brush has just been created by the user
		TRANSFORMING		// the brush is being transformed
	}

	//################## PROTECTED VARIABLES ##################//
	protected BoundingBox m_boundingBox = null;					// the brush's bounding box (/cuboid)
	protected BoundingBox m_cachedBoundingBox;					// used to cache the bounding box at the start of a transformation
	protected HandleState m_handleState = HandleState.RESIZE;	// the current handle type (we start off with resize handles)
	protected IRenderer m_renderer = null;						// the renderer associated with the current canvas on which
																// this brush is being manipulated

	//################## PRIVATE VARIABLES ##################//
	private State m_state;										// the current state of the brush
	private Transformation m_transformation = null;				// the current transformation being performed

	//################## CONSTRUCTORS ##################//
	/**
	Constructs a ResizableTranslatableBrush whose initial state depends on whether or not it's
	a new brush. By "new", I mean one the user is creating, as opposed to one which we're
	loading in, one which is being constructed as a result of a split operation, etc.

	@param isNew	Is the brush a new one that the user is creating?
	*/
	public ResizableTranslatableBrush(boolean isNew)
	{
		m_state = isNew ? State.INITIAL : State.IDLE;
	}

	//################## PROTECTED ABSTRACT METHODS ##################//
	/**
	Resizes the brush on the axes given by the AxisPair, so that the 2D projection onto those axes of
	its axis-aligned bounding box has the specified corners. (This sounds complicated, but intuitively
	it does exactly what we would expect, namely resizing two of the dimensions of the brush whilst
	leaving the other dimension unchanged. The two dimensions which change are those of the canvas on
	which we're doing the resizing.) Exactly how we do this depends on the brush type.

	@param corner0	A corner of the 2D projection of the new axis-aligned bounding box
	@param corner1	The opposite corner to corner0
	@param ap	The axis-pair of the canvas on which we're resizing
	*/
	protected abstract void resize(Point2d corner0, Point2d corner1, AxisPair ap);

	/**
	Translate the brush by the specified vector. Exactly how we do this depends on the brush type.

	@param trans	The vector by which to translate
	*/
	protected abstract void translate(Point3d trans);

	//################## PUBLIC METHODS ##################//
	/**
	Generates a Command that translates this brush by the specified offset.

	@param offset	The offset by which to translate this brush
	@return			As specified above
	*/
	public Command generate_translation_command(Point3d offset)
	{
		final Point3d offsetCopy = (Point3d)offset.clone();
		final Point3d negOffset = (Point3d)offset.clone();
		negOffset.negate();
		return new Command("Translation")
		{
			public void execute()
			{
				begin_transform();
				translate(offsetCopy);
				end_transform();				// FIXME: The bounding box recalculation in this can be optimized away.
			}

			public void undo()
			{
				begin_transform();
				translate(negOffset);
				end_transform();				// FIXME: Likewise.
			}
		};
	}

	/**
	This is the event handler which deals with mouse drags. In particular, it deals with a drag
	to level coordinates specified by p on a canvas with the specified associated renderer.

	@param renderer		The renderer associated with the canvas on which the mouse is being dragged
	@param p			The location of the mouse in level coordinates
	*/
	public void mouse_dragged(IRenderer renderer, Point2d p)
	{
		switch(m_state)
		{
			case TRANSFORMING:
			{
				m_transformation.transform(p);
				break;
			}
			case IDLE:
			{
				// If we somehow generated a mouse_dragged without calling mouse_pressed first
				// (perhaps by dragging onto this canvas from another component...users really
				// can be that annoying...), then we need to call mouse_pressed to set things up
				// and then recurse.
				if(mouse_pressed(renderer, p, LEFT_BUTTON, false))
				{
					mouse_dragged(renderer, p);
				}
				break;
			}
			default:
			{
				System.err.println("Brush dragged whilst in invalid state: " + m_state);
				break;
			}
		}
	}

	public void mouse_moved(IRenderer renderer, Point2d p)
	{
		set_correct_cursor(renderer, p);
	}

	/**
	This is the event handler which deals with mouse button presses over the brush.
	The immediate flag tells us whether or not the brush is being transformed as
	it's being selected - in that case, we always want to translate rather than do
	anything else.

	@param renderer		The renderer associated with the canvas on which the mouse was pressed
	@param p			The location of the mouse press in level coordinates
	@param mouseButton	Which mouse button was pressed
	@param immediate	Is this the start of an immediate transformation? (see above description)
	@return				A boolean indicating whether or not the brush thinks it should still be selected after returning
	*/
	public boolean mouse_pressed(IRenderer renderer, Point2d p, int mouseButton, boolean immediate)
	{
		switch(mouseButton)
		{
			case LEFT_BUTTON:
			{
				m_renderer = renderer;

				switch(m_state)
				{
					case INITIAL:
					{
						// Initially, users drag out the brush to the size they want. They use the 2nd corner (i.e.
						// the one opposite the 0th corner), because the 0th corner is located at the exact point of
						// creation and serves as the correct anchor for the resize.
						m_transformation = new HandleResizeTransformation(2);
						return true;
					}
					case IDLE:
					{
						if(immediate)
						{
							m_transformation = new TranslationTransformation(p);
							return true;
						}
						else
						{
							int nearbyHandle, nearbyEdge;
							if((nearbyHandle = find_nearby_handle(m_renderer, p)) != -1)
							{
								m_transformation = generate_handle_transformation(nearbyHandle);
								return true;
							}
							else if((nearbyEdge = find_nearby_edge(m_renderer, p)) != -1)
							{
								m_transformation = new EdgeResizeTransformation(nearbyEdge);
								return true;
							}
							else if(m_boundingBox.within_2D_bounds(p, m_renderer.get_axis_pair()))
							{
								m_transformation = new TranslationTransformation(p);
								return true;
							}
							else return false;
						}
					}
					default:
					{
						System.err.println("Brush pressed whilst in invalid state: " + m_state);
						return true;
					}
				}
			}
			case RIGHT_BUTTON:
			{
				right_mouse_pressed();
				return true;
			}
			default:
			{
				m_transformation = new NullTransformation();	// transformations can only be performed with the left mouse button
				return true;
			}
		}
	}

	public void mouse_released()
	{
		if(m_state == State.TRANSFORMING) m_transformation.execute_command();

		m_renderer = null;
		m_state = State.IDLE;
		m_transformation = null;
	}

	//################## PROTECTED METHODS ##################//
	/**
	This method is called at the start of a transformation to cache the current brush so that
	transformed versions can be calculated from it.
	*/
	protected void begin_transform()
	{
		m_cachedBoundingBox = m_boundingBox.clone();
	}

	/**
	Clears the brush's state.
	*/
	final protected void clear_state()
	{
		m_handleState = HandleState.RESIZE;
		m_renderer = null;
		m_state = State.IDLE;
		m_transformation = null;
	}

	/**
	This method is called at the end of a transformation to allow recalculation of things like
	bounding boxes.
	*/
	protected void end_transform() {}

	/**
	Generates a transformation based on the current handle state and the
	specified handle index.

	@param handleIndex	The index of the handle being manipulated by the user
	*/
	protected Transformation generate_handle_transformation(int handleIndex)
	{
		return new HandleResizeTransformation(handleIndex);
	}

	/**
	Renders the projection of the brush's bounding box using the specified renderer. Since we only
	render a brush's bounds when it's selected, this entails rendering the handles for manipulating
	the brush as well.

	<p><b>Preconditions:</b>
	<dl>
	<dd>The stroke is assumed to be set to the default stroke
	</dl>

	<p><b>Notes:</b>
	<ul>
	<li>Although render_bounds(renderer) is functionally equivalent to the call render_bounds(renderer, new BasicStroke()),
	the former is slightly more efficient in that the current stroke never gets modified.
	</ul>

	@param renderer	The renderer with which we want to render the brush's bounds
	*/
	protected void render_bounds(IRenderer renderer)
	{
		AxisAlignedBox.Projection proj = m_boundingBox.project_to_2D_using(renderer.get_axis_pair());
		renderer.draw_rectangle(proj.m_corners[0], proj.m_corners[1]);

		render_handles(renderer);
	}

	/**
	Renders the projection of the brush's bounding box using the specified renderer and stroke. Since we only
	render a brush's bounds when it's selected, this entails rendering the handles for manipulating the brush
	as well. (Note that the handles always get rendered using the default stroke, and that the stroke is always
	set to the default when the method returns.)

	@param renderer	The renderer with which we want to render the brush's bounds
	@param stroke	The stroke with which we want to render the brush's bounds
	*/
	protected void render_bounds(IRenderer renderer, Stroke stroke)
	{
		AxisAlignedBox.Projection proj = m_boundingBox.project_to_2D_using(renderer.get_axis_pair());

		renderer.set_stroke(stroke);
		renderer.draw_rectangle(proj.m_corners[0], proj.m_corners[1]);
		renderer.set_stroke(new BasicStroke());

		render_handles(renderer);
	}

	/**
	Renders the effects associated with the current transformation (if any).

	@param renderer	The renderer with which to render the effects
	*/
	final protected void render_transformation_effects(IRenderer renderer)
	{
		if(m_transformation != null) m_transformation.render_effects(renderer);
	}

	/**
	This is the event handler dealing with the right mouse button being pressed.
	*/
	protected void right_mouse_pressed()
	{
		m_transformation = new NullTransformation();	// transformations can only be performed with the left mouse button
	}

	//################## PRIVATE METHODS ##################//
	/**
	Looks for an edge near to the specified point.

	@param renderer	The renderer associated with the canvas on which to check the proximity between the point and edges
	@param p		A point in level coordinates
	@return			The index of the nearby edge, if any, or -1 otherwise
	*/
	private int find_nearby_edge(IRenderer renderer, Point2d p)
	{
		Point2d[] cornerHandles = new Point2d[4];
		for(int i=0; i<4; ++i) cornerHandles[i] = m_boundingBox.get_corner_handle(renderer.get_axis_pair(), i);

		for(int i=0; i<4; ++i)
		{
			int j = (i+1)%4;
			if(i%2 == 0)
			{
				double minX, maxX;
				if(cornerHandles[i].x < cornerHandles[j].x)	{ minX = cornerHandles[i].x; maxX = cornerHandles[j].x; }
				else										{ minX = cornerHandles[j].x; maxX = cornerHandles[i].x; }
				if(BrushUtil.near_edge_y(renderer, p, cornerHandles[i].y, minX, maxX)) return i;
			}
			else
			{
				double minY, maxY;
				if(cornerHandles[i].y < cornerHandles[j].y)	{ minY = cornerHandles[i].y; maxY = cornerHandles[j].y; }
				else										{ minY = cornerHandles[j].y; maxY = cornerHandles[i].y; }
				if(BrushUtil.near_edge_x(renderer, p, cornerHandles[i].x, minY, maxY)) return i;
			}
		}
		return -1;
	}

	/**
	Looks for a handle near to the specified point.

	@param renderer		The renderer associated with the canvas on which to check the proximity between the point and handles
	@param p			A point in level coordinates
	@return				The index of the nearby handle, if any, or -1 otherwise
	*/
	private int find_nearby_handle(IRenderer renderer, Point2d p)
	{
		AxisPair ap = renderer.get_axis_pair();

		switch(m_handleState)
		{
			case RESIZE:
			case ROTATE:
			{
				for(int i=0; i<4; ++i)
				{
					Point2d cornerHandle = m_boundingBox.get_corner_handle(ap, i);
					if(BrushUtil.near_handle(renderer, p, cornerHandle.x, cornerHandle.y)) return i;
				}
				break;
			}
			case SHEAR:
			{
				for(int i=0; i<4; ++i)
				{
					Point2d edgeHandle = m_boundingBox.get_edge_handle(ap, i);
					if(BrushUtil.near_handle(renderer, p, edgeHandle.x, edgeHandle.y)) return i;
				}
				break;
			}
		}
		return -1;
	}

	/**
	Renders the handles which the user can grab to resize, rotate or shear the brush. Which
	handles are rendered depends on the handle state we're currently in.

	@param renderer	The renderer associated with the canvas onto which the handles are to be rendered
	*/
	private void render_handles(IRenderer renderer)
	{
		Color[] handleColours = new Color[] {Color.magenta,Color.cyan,Color.pink,Color.orange};

		AxisPair ap = renderer.get_axis_pair();

		switch(m_handleState)
		{
			case RESIZE:
			{
				for(int i=0; i<4; ++i)
				{
					Point2d p = m_boundingBox.get_corner_handle(ap, i);
					renderer.set_colour(handleColours[i]);
					BrushUtil.render_square_handle(renderer, p);
				}
				break;
			}
			case ROTATE:
			{
				for(int i=0; i<4; ++i)
				{
					Point2d p = m_boundingBox.get_corner_handle(ap, i);
					renderer.set_colour(handleColours[i]);
					BrushUtil.render_circular_handle(renderer, p);
				}
				break;
			}
			case SHEAR:
			{
				for(int i=0; i<4; ++i)
				{
					Point2d p = m_boundingBox.get_edge_handle(ap, i);
					renderer.set_colour(handleColours[i]);
					BrushUtil.render_diamond_handle(renderer, p);
				}
				break;
			}
		}
	}

	/**
	The currently selected brush has the responsibility to decide what the cursor on the
	active canvas looks like. If the cursor is near one of the brush's handles, for
	instance, we want the cursor to be a resize cursor. This method decides which cursor
	should be set and sets it accordingly.

	@param renderer	The renderer associated with the canvas on which we want to set the cursor
	@param p_Coords	The location of the mouse in level coordinates on the canvas
	*/
	private void set_correct_cursor(IRenderer renderer, Point2d p_Coords)
	{
		// FIXME: This function is a mess.

		renderer.set_cursor(new Cursor(Cursor.DEFAULT_CURSOR));

		AxisAlignedBox.Projection proj = m_boundingBox.project_to_2D_using(renderer.get_axis_pair());
		boolean hAxisInverted = renderer.is_inverted_axis(renderer.axis_h());
		boolean vAxisInverted = renderer.is_inverted_axis(renderer.axis_v());
		double left = hAxisInverted ? proj.m_corners[1].x : proj.m_corners[0].x;
		double right = hAxisInverted ? proj.m_corners[0].x : proj.m_corners[1].x;
		double top = vAxisInverted ? proj.m_corners[1].y : proj.m_corners[0].y;
		double bottom = vAxisInverted ? proj.m_corners[0].y : proj.m_corners[1].y;

		if(BrushUtil.near_edge_x(renderer, p_Coords, left, proj.m_corners[0].y, proj.m_corners[1].y))
			renderer.set_cursor(new Cursor(Cursor.W_RESIZE_CURSOR));
		if(BrushUtil.near_edge_x(renderer, p_Coords, right, proj.m_corners[0].y, proj.m_corners[1].y))
			renderer.set_cursor(new Cursor(Cursor.E_RESIZE_CURSOR));
		if(BrushUtil.near_edge_y(renderer, p_Coords, top, proj.m_corners[0].x, proj.m_corners[1].x))
			renderer.set_cursor(new Cursor(Cursor.N_RESIZE_CURSOR));
		if(BrushUtil.near_edge_y(renderer, p_Coords, bottom, proj.m_corners[0].x, proj.m_corners[1].x))
			renderer.set_cursor(new Cursor(Cursor.S_RESIZE_CURSOR));

		switch(m_handleState)
		{
			case RESIZE:
			{
				if(BrushUtil.near_handle(renderer, p_Coords, left, top))
					renderer.set_cursor(new Cursor(Cursor.NW_RESIZE_CURSOR));
				if(BrushUtil.near_handle(renderer, p_Coords, left, bottom))
					renderer.set_cursor(new Cursor(Cursor.SW_RESIZE_CURSOR));
				if(BrushUtil.near_handle(renderer, p_Coords, right, top))
					renderer.set_cursor(new Cursor(Cursor.NE_RESIZE_CURSOR));
				if(BrushUtil.near_handle(renderer, p_Coords, right, bottom))
					renderer.set_cursor(new Cursor(Cursor.SE_RESIZE_CURSOR));
				break;
			}
			case ROTATE:
			{
				if(BrushUtil.near_handle(renderer, p_Coords, left, top) ||
				   BrushUtil.near_handle(renderer, p_Coords, left, bottom) ||
				   BrushUtil.near_handle(renderer, p_Coords, right, top) ||
				   BrushUtil.near_handle(renderer, p_Coords, right, bottom))
				{
					renderer.set_cursor(new Cursor(Cursor.CROSSHAIR_CURSOR));
				}
				break;
			}
			case SHEAR:
			{
				double midX = (left + right) / 2;
				double midY = (top + bottom) / 2;
				if(BrushUtil.near_handle(renderer, p_Coords, left, midY) || BrushUtil.near_handle(renderer, p_Coords, right, midY))
					renderer.set_cursor(new Cursor(Cursor.N_RESIZE_CURSOR));
				if(BrushUtil.near_handle(renderer, p_Coords, midX, top) || BrushUtil.near_handle(renderer, p_Coords, midX, bottom))
					renderer.set_cursor(new Cursor(Cursor.E_RESIZE_CURSOR));
				break;
			}
		}
	}

	//################## NESTED CLASSES ##################//
	protected abstract class Transformation
	{
		/**
		Constructing a new transformation triggers a change of state to the TRANSFORMING state.
		*/
		public Transformation()
		{
			m_state = State.TRANSFORMING;
		}

		/**
		Render any effects associated with the transformation, e.g. for translation
		this would include the translation anchor.
		*/
		public void render_effects(IRenderer renderer) {}

		/**
		Finish the transformation by executing a command (thus allowing the transformation to be undone).
		*/
		public abstract void execute_command();

		/**
		Determine the transformation required when the user has dragged to point p,
		and apply it to the brush.

		@param p	The mouse location in level coordinates
		*/
		public abstract void transform(Point2d p);
	}

	final protected class NullTransformation extends Transformation
	{
		public void execute_command()		{}
		public void transform(Point2d p)	{}
	}

	private abstract class ResizeTransformation extends Transformation
	{
		protected Axis m_fixedAxis;				// if we're dragging the left edge, for instance, then Y is the fixed axis
		protected double m_fixedCoord;			// for edge-resizing: the coordinate on the fixed axis gets stored in this variable
		protected int m_handleIndex;			// the index of the handle we're using to resize (note that if we're doing edge-resizing,
												// there will still be such a handle, it's just that we only allow changes on one of the
												// axes)
		protected Point2d m_fixedCorner;		// the location of the fixed corner (the one opposite to the one we're dragging around)
		protected Point2d m_variableCorner;		// the current location of the variable corner (the one we're dragging around)
		protected Point2d m_variableCornerOld;	// the original location of the variable corner at the start of the resize

		public ResizeTransformation()
		{
			begin_transform();
		}

		public void execute_command()
		{
			final AxisPair ap = m_renderer.get_axis_pair();
			final Point2d fixedCorner = (Point2d)m_fixedCorner.clone();
			final Point2d variableCorner = (Point2d)m_variableCorner.clone();
			final Point2d variableCornerOld = (Point2d)m_variableCornerOld.clone();
			CommandManager.instance().execute_compressible_command(new Command("Resize")
				{
					public void execute()
					{
						begin_transform();
						resize(fixedCorner, variableCorner, ap);
						end_transform();
					}

					public void undo()
					{
						begin_transform();
						resize(fixedCorner, variableCornerOld, ap);
						end_transform();
					}
				},
				Pair.make_pair("Initial Brush Creation", "Brush Creation"),
				Pair.make_pair("Initial New Selection", "New Selection"));
		}

		public void transform(Point2d p)
		{
			if(Options.is_set("Snap To Grid"))
			{
				p = m_renderer.find_nearest_grid_intersect_in_coords(p);
			}

			m_variableCorner = (Point2d)p.clone();

			// Handle edge-resizing.
			switch(m_fixedAxis)
			{
				case NONE:
				{
					break;
				}
				case X:
				{
					m_variableCorner.x = m_fixedCoord;
					break;
				}
				case Y:
				{
					m_variableCorner.y = m_fixedCoord;
					break;
				}
			}

			// Prevent the dimensions from ever becoming smaller than a given amount.
			Point2d dimensions = MathUtil.subtract(m_variableCorner, m_fixedCorner);
			if(0 <= dimensions.x && dimensions.x < MINIMUM_BRUSH_DIMENSION) m_variableCorner.x = m_fixedCorner.x + MINIMUM_BRUSH_DIMENSION;
			else if(-MINIMUM_BRUSH_DIMENSION < dimensions.x && dimensions.x < 0) m_variableCorner.x = m_fixedCorner.x - MINIMUM_BRUSH_DIMENSION;
			if(0 <= dimensions.y && dimensions.y < MINIMUM_BRUSH_DIMENSION) m_variableCorner.y = m_fixedCorner.y + MINIMUM_BRUSH_DIMENSION;
			else if(-MINIMUM_BRUSH_DIMENSION < dimensions.y && dimensions.y < 0) m_variableCorner.y = m_fixedCorner.y - MINIMUM_BRUSH_DIMENSION;

			resize(m_fixedCorner, m_variableCorner, m_renderer.get_axis_pair());
			set_correct_cursor(m_renderer, p);
		}
	}

	private class EdgeResizeTransformation extends ResizeTransformation
	{
		/**
		Constructs an edge resize transformation for the brush.

		@param edgeIndex	The index of the edge with which we're going to resize
		*/
		public EdgeResizeTransformation(int edgeIndex)
		{
			AxisPair ap = m_renderer.get_axis_pair();

			if(edgeIndex % 2 == 0)
			{
				m_fixedAxis = Axis.X;
				m_fixedCoord = m_boundingBox.get_corner_handle(ap, edgeIndex).x;
			}
			else
			{
				m_fixedAxis = Axis.Y;
				m_fixedCoord = m_boundingBox.get_corner_handle(ap, edgeIndex).y;
			}
			m_handleIndex = edgeIndex;
			m_fixedCorner = m_boundingBox.get_opposite_corner_handle(ap, edgeIndex);
			m_variableCorner = m_boundingBox.get_corner_handle(ap, edgeIndex);
			m_variableCornerOld = m_boundingBox.get_corner_handle(ap, edgeIndex);
		}
	}

	final protected class HandleResizeTransformation extends ResizeTransformation
	{
		/**
		Constructs a handle resize transformation for the brush.

		@param handleIndex	The index of the handle with which we're going to resize
		*/
		public HandleResizeTransformation(int handleIndex)
		{
			AxisPair ap = m_renderer.get_axis_pair();

			m_fixedAxis = Axis.NONE;
			m_handleIndex = handleIndex;
			m_fixedCorner = m_boundingBox.get_opposite_corner_handle(ap, handleIndex);
			m_variableCorner = m_boundingBox.get_corner_handle(ap, handleIndex);
			m_variableCornerOld = m_boundingBox.get_corner_handle(ap, handleIndex);
		}
	}

	private class TranslationTransformation extends Transformation
	{
		final private static int TRANSLATION_ANCHOR_SIZE = 8;	// the size of the translation anchor

		private Point2d m_anchor;				// the location of the translation anchor (used for rendering, not in the actual transformation!)
		private Point2d m_dragOffset;			// the offset of the point we clicked from the top-left corner (on our current canvas)
		private Point2d m_fixedCorner;			// the location of the top-left corner (top-left on our current canvas, of course)
		private Point3d m_offset;				// the offset by which we've translated at present

		/**
		Constructs a translation transformation for the brush.

		@param p	The point clicked by the user (in level coordinates)
		*/
		public TranslationTransformation(Point2d p)
		{
			begin_transform();

			// Figure out which corner handle we're nearest to, and let that be the one
			// we snap to the grid. The "Manhattan" distance |dx| + |dy| will give us
			// what we want in this instance without any need for expensive distance
			// computations.
			m_fixedCorner = m_boundingBox.find_manhattan_nearest_corner_handle(m_renderer.get_axis_pair(), p);

			m_dragOffset = MathUtil.subtract(p, m_fixedCorner);
			m_anchor = m_fixedCorner;
			m_offset = new Point3d(0,0,0);
		}

		public void execute_command()
		{
			if(m_offset.equals(new Point3d(0,0,0))) return;	// don't bother with no-op translations

			translate(new Point3d(0,0,0));	// reset the brush to its pre-translation state

			CommandManager.instance().execute_command(generate_translation_command(m_offset));
		}

		public void render_effects(IRenderer renderer)
		{
			/*
			Renders the translation anchor, if the specified renderer is the one associated with the canvas
			on which we're manipulating the brush. Otherwise does nothing. In other words, we only render
			the translation anchor on the canvas on which we're dragging the brush, because the translation
			anchor is a 2D point rather than a 3D one - we know where it is on the canvas in question, but
			we don't know its missing 3D component and hence can't render it on the other canvases.
			*/
			if(renderer == m_renderer)
			{
				renderer.set_colour(Color.white);

				Point2d[] p = new Point2d[] {	renderer.add_pixel_offset(m_anchor, -TRANSLATION_ANCHOR_SIZE, -TRANSLATION_ANCHOR_SIZE),
												renderer.add_pixel_offset(m_anchor, TRANSLATION_ANCHOR_SIZE, TRANSLATION_ANCHOR_SIZE),
												renderer.add_pixel_offset(m_anchor, TRANSLATION_ANCHOR_SIZE, -TRANSLATION_ANCHOR_SIZE),
												renderer.add_pixel_offset(m_anchor, -TRANSLATION_ANCHOR_SIZE, TRANSLATION_ANCHOR_SIZE)	};
				renderer.draw_line(p[0], p[1]);
				renderer.draw_line(p[2], p[3]);
			}
		}

		public void transform(Point2d p)
		{
			/*
			Method:
			newFixedCorner_Coords = p - m_dragOffset
			Snap the above(?)
			trans = newFixedCorner - m_fixedCorner
			Cache the new translation anchor
			Translate
			*/

			Point2d newFixedCorner = MathUtil.subtract(p, m_dragOffset);

			if(Options.is_set("Snap To Grid"))
			{
				// It's the fixed corner of the box which we want to snap to the grid.
				newFixedCorner = m_renderer.find_nearest_grid_intersect_in_coords(newFixedCorner);
			}

			Point2d trans = MathUtil.subtract(newFixedCorner, m_fixedCorner);

			m_anchor = newFixedCorner;

			m_offset = m_renderer.get_axis_pair().generate_3D_point(trans,0);
			translate(m_offset);
		}
	}
}