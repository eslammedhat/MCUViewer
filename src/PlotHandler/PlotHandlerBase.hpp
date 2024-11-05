#ifndef _PLOTHANDLERBASE_HPP
#define _PLOTHANDLERBASE_HPP

#include <chrono>
#include <map>
#include <mutex>
#include <thread>

#include "CSVStreamer.hpp"
#include "Plot.hpp"
#include "ScrollingBuffer.hpp"
#include "StlinkDebugProbe.hpp"
#include "spdlog/spdlog.h"

class PlotHandlerBase
{
   public:
	enum class state
	{
		STOP = 0,
		RUN = 1,
	};

	PlotHandlerBase(std::atomic<bool>& done, std::mutex* mtx, spdlog::logger* logger);
	virtual ~PlotHandlerBase() = default;

	std::shared_ptr<Plot> addPlot(const std::string& name);
	bool removePlot(const std::string& name);
	bool renamePlot(const std::string& oldName, const std::string& newName);
	bool removeAllPlots();
	std::shared_ptr<Plot> getPlot(std::string name);
	virtual std::string getLastReaderError() const = 0;
	bool eraseAllPlotData();
	void setViewerState(state state);
	state getViewerState() const;
	uint32_t getVisiblePlotsCount() const;
	uint32_t getPlotsCount() const;
	bool checkIfPlotExists(const std::string& name) const;
	void setMaxPoints(uint32_t maxPoints);

	/* TODO these functions do not belong here */
	double castToProperType(uint32_t value, Variable::Type type);
	uint32_t applyTransformations(uint32_t value, uint32_t shift, uint32_t mask);
	class iterator
	{
	   public:
		using iterator_category = std::forward_iterator_tag;
		explicit iterator(std::map<std::string, std::shared_ptr<Plot>>::iterator iter);
		iterator& operator++();
		iterator operator++(int);
		bool operator==(const iterator& other) const;
		bool operator!=(const iterator& other) const;
		std::shared_ptr<Plot> operator*();

	   private:
		std::map<std::string, std::shared_ptr<Plot>>::iterator m_iter;
	};

	iterator begin();
	iterator end();

   protected:
	std::unique_ptr<CSVStreamer> csvStreamer;
	std::atomic<bool>& done;
	std::atomic<state> viewerState = state::STOP;
	std::map<std::string, std::shared_ptr<Plot>> plotsMap;
	std::mutex* mtx;
	std::thread dataHandle;
	std::atomic<bool> stateChangeOrdered = false;
	spdlog::logger* logger;
};

#endif