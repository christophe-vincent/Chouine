#! /usr/bin/env python3
# -*- coding: utf-8 -*-
import os
import time
import boto3
from botocore.exceptions import NoCredentialsError, ClientError

# Configuration
project_dir = os.path.dirname(os.path.dirname(os.path.realpath(__file__)))
local_folder = os.path.join(project_dir, "Godot", "Exports")
bucket_name = 'jeu-chouine'
s3_prefix = 'rc'

distribution_id = 'E2WL504EAYVGS3'
paths_to_invalidate = ['/*']

# Initialize S3 session
session = boto3.Session()
s3 = session.client('s3')


def upload_files(folder, bucket, prefix=''):
    nb_files = 0
    for root, _, files in os.walk(folder):
        for file in files:
            nb_files += 1
            local_path = os.path.join(root, file)
            relative_path = os.path.relpath(local_path, folder)
            s3_key = os.path.join(prefix, relative_path).replace("\\", "/")
            if s3_key.endswith('.DS_Store'):
                continue  # Skip macOS system files
            if s3_key.endswith('html'):
                extra_args = {'ContentType': 'text/html'}
            elif s3_key.endswith('css'):
                extra_args = {'ContentType': 'text/css'}
            elif s3_key.endswith('js'):
                extra_args = {'ContentType': 'application/javascript'}
            else:
                extra_args = {}

            try:
                s3.upload_file(local_path, bucket, s3_key, ExtraArgs=extra_args)
                print(f"✅ Uploaded: {local_path} → s3://{bucket}/{s3_key}")
            except FileNotFoundError:
                print(f"❌ File not found: {local_path}")
            except NoCredentialsError:
                print("❌ AWS credentials not found.")
                return
            except ClientError as e:
                print(f"❌ Upload failed: {e}")
    if nb_files == 0:
        print(f"Aucun fichier trouvé dans le répertoire {folder}")


# Run the upload
upload_files(local_folder, bucket_name, s3_prefix)


client = boto3.client('cloudfront')

try:
    response = client.create_invalidation(
        DistributionId=distribution_id,
        InvalidationBatch={
            'Paths': {
                'Quantity': len(paths_to_invalidate),
                'Items': paths_to_invalidate
            },
            'CallerReference': str(hash(frozenset(paths_to_invalidate)))  # Identifiant unique
        }
    )
    invalidation_id = response['Invalidation']['Id']
except ClientError as e:
    print(f"❌ Erreur lors de l'invalidation : {e}")

print(f"🕒 Invalidation lancée : {invalidation_id}")

# Attendre que l'invalidation soit terminée
while True:
    status_response = client.get_invalidation(
        DistributionId=distribution_id,
        Id=invalidation_id
    )
    status = status_response['Invalidation']['Status']
    print(f"⏳ Statut actuel : {status}")

    if status == 'Completed':
        print("✅ Invalidation terminée.")
        break

    time.sleep(10)