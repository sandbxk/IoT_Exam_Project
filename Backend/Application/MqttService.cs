using Application.Interfaces;

namespace Application;

public class MqttService
{
    private readonly IMqttRepository _repository;

    public MqttService(IMqttRepository repository)
    {
        _repository = repository ?? throw new ArgumentNullException(nameof(repository));
    }
}