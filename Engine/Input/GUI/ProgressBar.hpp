
//==============================================================================================================
//ProgressBar.hpp
//by Albert Chen Sep-1-2015.
//==============================================================================================================

#include "Engine\Renderer\OGLRenderingUtils.hpp"
#include "Engine\Renderer\OpenGLRenderer.hpp"

#include "UIElement.hpp"

const Vector2 DEFAULT_PROGRESS_BAR_SIZE = Vector2(512.0f, 32.0f);

class ProgressBar : public UIElement{
public:
	//vars
	float m_percent;

	//methods
	//constructors
	ProgressBar();
	ProgressBar(const Vector2& screenPosition, const Vector2& newSize = DEFAULT_PROGRESS_BAR_SIZE , const Vector2& scaleFactor = Vector2::ONE, const Rgba& elementColor = Rgba::NEON_GREEN );
	~ProgressBar();
	//setters
	void SetDefaultProgressBarMaterial();

	void ScaleRenderBoundsByPercentAlongX(double deltaSeconds);

	//upkeep
	void Update(double deltaSeconds, const float& percent = 1.0f);

	void RenderMesh(OpenGLRenderer* renderer);


	

	






};