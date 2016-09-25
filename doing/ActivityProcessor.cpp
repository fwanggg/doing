#include "ActivityProcessor.h"
ActivityProcessor::~ActivityProcessor() {} //does nothing yet
ActivityProcessor::ActivityProcessor(ConcurrentQueue<Activity>& _job_queue) : m_job_queue(_job_queue)
{}