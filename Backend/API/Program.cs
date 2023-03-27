using Application;
using Application.Interfaces;
using Infrastructure;
using Microsoft.EntityFrameworkCore;

var builder = WebApplication.CreateBuilder(args);

// Add services to the container.

builder.Services.AddControllers();
// Learn more about configuring Swagger/OpenAPI at https://aka.ms/aspnetcore/swashbuckle
builder.Services.AddEndpointsApiExplorer();
builder.Services.AddSwaggerGen();

builder.Services.AddDbContext<DatabaseContext>(options =>
{
    options.UseSqlite(
        "Data Source=db.db"
    );
    options.EnableSensitiveDataLogging();
    options.EnableDetailedErrors();
});

Infrastructure.DependencyResolver.DependencyResolverService.RegisterInfrastructureLayer(builder.Services);
Application.DependencyResolver.DependendencyResolverService.RegisterApplicationLayer(builder.Services);

MqttService mqttService = new MqttService(new MqttRepository(new DatabaseContext(
    new DbContextOptionsBuilder<DatabaseContext>().UseSqlite("Data Source=db.db").Options))
);

mqttService.Connect();
mqttService.Subscribe("door/open");
mqttService.Subscribe("door/close");

var app = builder.Build();

// Configure the HTTP request pipeline.
if (app.Environment.IsDevelopment())
{
    app.UseSwagger();
    app.UseSwaggerUI();
}

app.UseHttpsRedirection();

app.UseAuthorization();

app.MapControllers();

app.Run();