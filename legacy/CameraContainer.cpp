#include "CameraContainer.h"
#include "imgui/imgui.h"
#include "Camera.h"
#include "Graphics.h"
#include "RenderGraph.h"

#include <sstream>

Camera* CameraContainer::operator->()
{
	return &GetActiveCamera();
}

CameraContainer::CameraContainer(AppLog* aLog)
	:
	applog(aLog)
{}

CameraContainer::~CameraContainer()
{}

void CameraContainer::LinkTechniques( Rgph::RenderGraph& rg )
{
	for(int i = 0; i < cameras.size(); i++)
	{
		cameras[i]->LinkTechniques(rg);

		std::ostringstream oss;
		oss << "Добавлено к рендеру [" << cameras[i]->GetName() << "]\n";

		applog->AddLog(CAMERAS_LOG, oss.str().c_str());
	}
}

void CameraContainer::Submit( size_t channels ) const
{
	for( size_t i = 0; i < cameras.size(); i++ )
	{
		if( i != active )
		{
			cameras[i]->Submit( channels );
		}
	}
}

void CameraContainer::Bind(Graphics& gfx)
{
	gfx.SetCamera((*this)->GetMatrix());
}

void CameraContainer::AddCamera(std::shared_ptr<Camera> pCam)
{
	cameras.push_back(std::move(pCam));

	std::ostringstream oss;
	oss << "Добавлено [" << cameras.at(cameras.size() - 1)->GetName() << "]\n";

	applog->AddLog(CAMERAS_LOG, oss.str().c_str());
}

Camera& CameraContainer::GetActiveCamera()
{
	return *cameras[active];
}

void CameraContainer::ShowCamsInformationAndSettings(Graphics& gfx)
{
	if (ImGui::Begin("Камеры", NULL,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		if (ImGui::BeginCombo("Акт. камера", (*this)->GetName().c_str()))
		{
			for (int i = 0; i < std::size(cameras); i++)
			{
				const bool isSelected = i == active;
				if (ImGui::Selectable(cameras[i]->GetName().c_str(), isSelected))
				{
					active = i;
				}
			}
			ImGui::EndCombo();
		}

		if (ImGui::BeginCombo("Упр. камера", GetControlledCamera().GetName().c_str()))
		{
			for (int i = 0; i < std::size(cameras); i++)
			{
				const bool isSelected = i == controlled;
				if (ImGui::Selectable(cameras[i]->GetName().c_str(), isSelected))
				{
					controlled = i;
				}
			}
			ImGui::EndCombo();
		}

		GetControlledCamera().SpawnControlWidgets(gfx);
	}
	ImGui::End();
}

Camera& CameraContainer::GetControlledCamera()
{
	return *cameras[controlled];
}