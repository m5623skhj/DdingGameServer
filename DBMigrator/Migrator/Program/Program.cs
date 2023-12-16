using FluentMigrator.Runner;
using FluentMigrator.Runner.Initialization;
using FluentMigrator.Runner.Processors.MySql;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Logging;
using System;
using System.IO;
using System.Reflection;

namespace Migrator.Program
{
    class Program
    {
        static int Main(string[] args)
        {
            var servicesProvider = CreateServices();

            using var scope = servicesProvider.CreateScope();
            return UpdateDatabase(scope.ServiceProvider);
        }

        private static IServiceProvider CreateServices()
        {
            string connectionString = "server=.;database=test;UID=my_test;password=1234";

            return new ServiceCollection()
                .AddFluentMigratorCore()
                .ConfigureRunner(rb => rb
                    .AddSqlServer()
                    .WithGlobalConnectionString(connectionString)
                    .WithGlobalStripComments(false)
                    .ScanIn(Assembly.GetExecutingAssembly())
                        .For.Migrations()
                        .For.EmbeddedResources())
                .AddLogging(lb => lb.AddFluentMigratorConsole())
                .BuildServiceProvider(false);
        }

        private static int UpdateDatabase(IServiceProvider serviceProvider)
        {
            var runner = serviceProvider.GetRequiredService<IMigrationRunner>();

            try
            {
                runner.MigrateUp();
                return 0;
            }
            catch (Exception ex)
            {
                Console.WriteLine("Migration failed : " + ex.Message);
                return 1;
            }
        }
    }
}
