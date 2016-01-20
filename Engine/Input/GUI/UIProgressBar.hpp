//==============================================================================================================
//ProgressBar.hpp
//by Albert Chen Sep-1-2015.
//==============================================================================================================

#include "Engine\Renderer\OGLRenderingUtils.hpp"
#include "Engine\Renderer\OpenGLRenderer.hpp"

#include "UIElement.hpp"

const Vector2 DEFAULT_PROGRESS_BAR_SIZE = Vector2(512.0f, 32.0f);

class UIProgressBar : public UIElement{
public:


	//methods
	//constructors
	UIProgressBar();
	UIProgressBar(const Vector2& screenPosition, const Vector2& newSize = DEFAULT_PROGRESS_BAR_SIZE , const Vector2& scaleFactor = Vector2::ONE, const Rgba& elementColor = Rgba::NEON_GREEN );
	//setters
	void SetDefaultProgressBarMaterial();

	void ScaleRenderBoundsByPercentAlongX(double deltaSeconds);
	void ScaleRenderBoundsByPercentAlongY(double deltaSeconds);

	void SetScaleAlongX(){ m_scaleAlongX = true; }
	void SetScaleAlongY(){ m_scaleAlongX = false; }
	//upkeep
	void Update(double deltaSeconds, const float& percent = 1.0f);

	void RenderMesh(OpenGLRenderer* renderer, ModelViewMatrix* modelMat = NULL);

protected:
	//vars
	float m_percent;
	AABB2 m_fullBarRenderBounds;
	bool m_scaleAlongX = true;//false means scale along Y

};

typedef std::vector<UIProgressBar*> UIProgressBars;
typedef std::vector<UIProgressBar*>::iterator UIProgressBarsIterator;

//===========================================================================================================

